*This project has been created as part of the 42 curriculum by haito.*

# Inception

## Description

Inception is a system administration project: it sets up a small web
infrastructure with Docker Compose inside a virtual machine. The result is a
WordPress website served at `https://haito.42.fr`.

The infrastructure is made of three services, each running in its own container
built from `debian:bookworm` with a Dockerfile written for this project:

- **nginx**: the only entry point, on port 443 with TLSv1.2 or TLSv1.3;
- **wordpress**: WordPress run by php-fpm (no web server inside);
- **mariadb**: the database used by WordPress.

The containers talk to each other over a dedicated Docker network. The database
and the website files are stored in two Docker named volumes whose data lives in
`/home/haito/data` on the host, so the site survives restarts and reboots.

```
             host :443 (HTTPS)
                    |
+------------------ | ------- Docker network: inception_net ------------------+
|                   v                                                          |
|  +---------+  FastCGI :9000  +-------------+  MySQL protocol :3306  +---------+ |
|  |  nginx  | --------------> |  wordpress  | ---------------------> | mariadb | |
|  +---------+                 +-------------+                        +---------+ |
+-------|---------------------------|-----------------------------------|-------+
        | (read-only)               |                                   |
        +------ wordpress_data -----+                              mariadb_data
           /home/haito/data/wordpress                      /home/haito/data/mariadb
```

## Instructions

Prerequisites: a Linux VM with Docker Engine, the Docker Compose plugin and `make`.
Full details are in [DEV_DOC.md](DEV_DOC.md); day-to-day use is described in
[USER_DOC.md](USER_DOC.md).

1. Point the domain to the local machine:
   ```sh
   echo "127.0.0.1 haito.42.fr" | sudo tee -a /etc/hosts
   ```
2. Create the configuration file and fill in every value:
   ```sh
   cp srcs/.env.example srcs/.env
   ```
3. Create the secrets (ignored by git):
   - `secrets/db_root_password.txt`: MariaDB root password
   - `secrets/db_password.txt`: MariaDB password of the WordPress user
   - `secrets/credentials.txt`: `WP_ADMIN_PASSWORD=...` and `WP_USER_PASSWORD=...`

   Database passwords must be 16 to 128 characters long and contain a lowercase
   letter, an uppercase letter and a digit.
4. Build and start everything:
   ```sh
   make
   ```
5. Open https://haito.42.fr and accept the self-signed certificate warning.
   The administration panel is at https://haito.42.fr/wp-admin/.

Other targets: `make stop` / `make start`, `make down` (remove the containers,
keep the data), `make clean` (also remove volumes and images, keep the files in
`/home/haito/data`), `make fclean` (also delete the data), `make re`.

## Project description

### Use of Docker and sources in this project

Each service has its own image, built by Docker Compose from a Dockerfile in
`srcs/requirements/<service>/`. Only the `debian:bookworm` base image is pulled;
nothing else comes from Docker Hub. Each image has the same name as its service
(`mariadb:inception`, `wordpress:inception`, `nginx:inception`).
The `Makefile` drives Docker Compose, and Docker Compose builds the images and
creates the containers, the network, the volumes and the secrets.

```
.
├── Makefile                      # make / down / clean / fclean / ...: calls docker compose
├── README.md, USER_DOC.md, DEV_DOC.md
├── secrets/                      # passwords, ignored by git, mounted as Docker secrets
│   ├── credentials.txt           # WordPress user passwords
│   ├── db_password.txt           # MariaDB password of the WordPress user
│   └── db_root_password.txt      # MariaDB root password
└── srcs/
    ├── docker-compose.yml        # services, network, volumes, secrets
    ├── .env.example              # template for .env (non-secret settings)
    ├── .env                      # ignored by git
    └── requirements/
        ├── mariadb/
        │   ├── Dockerfile
        │   ├── conf/60-inception.cnf     # listen on all interfaces, port 3306
        │   └── tools/db_setup.sh         # validate, initialise, configure, then exec mariadbd
        ├── wordpress/
        │   ├── Dockerfile
        │   ├── conf/www.conf             # php-fpm pool listening on port 9000
        │   └── tools/wp_setup.sh         # download, configure and install WordPress, then exec php-fpm
        └── nginx/
            ├── Dockerfile
            ├── conf/nginx.conf           # HTTPS server on 443, TLSv1.2/1.3, FastCGI to wordpress:9000
            └── tools/nginx_setup.sh      # create the self-signed certificate, then exec nginx
```

### Main design choices

#### Common to all images

- **Base image: `debian:bookworm`.** The subject requires the penultimate stable
  release of Debian or Alpine. According to the
  [Debian releases page](https://www.debian.org/releases/), the current stable
  release is trixie (13), so the penultimate one is bookworm (12).
- **Package versions come from bookworm.** MariaDB, NGINX and PHP are installed with
  `apt-get` and are not pinned to a version: the bookworm repositories already
  provide one fixed version of each package.
- **`apt-get` instead of `apt`.** `apt` is meant for interactive use and its output
  format is not stable (it even prints a warning when used in scripts).
  `apt-get` is the recommended tool in scripts and Dockerfiles.
- **`apt-get update -e any`.** Without `-e any`, `apt-get update` can exit with
  status 0 even if some package lists failed to download. With it, any download
  error makes the build fail immediately.
- **Setup script as `ENTRYPOINT`, main process as `CMD`.**
  Each image runs a setup script first (`db_setup.sh`, `wp_setup.sh`,
  `nginx_setup.sh`). The script ends with `exec "$@"`, which replaces the shell
  with the command given in `CMD`. As a result, the main process
  (`mariadbd`, `php-fpm8.2 -F`, `nginx -g 'daemon off;'`) runs as PID 1, receives
  the signals sent by `docker stop`, and no shell or loop stays in front of it.
  The setup logic (validation and first-time initialisation) is the responsibility
  of the script; how the service runs is defined in the Dockerfile.
- **POSIX `sh` instead of `bash`.** The scripts need no Bash-specific features.
  They use `set -e` so that an unexpected failure stops the container; commands
  whose failure is expected are run inside `if` or `until` and handled there.
- **Validation before start.** The scripts check the environment variables and
  secrets first and stop with a clear `[ERROR]` message if something is missing
  or invalid, instead of failing later with a less readable MariaDB or WordPress
  error.

#### MariaDB

- The setup script follows the logic of the
  [official MariaDB image entrypoint](https://github.com/MariaDB/mariadb-docker/blob/master/docker-entrypoint.sh).
- **First-time initialisation.** The package creates the system tables at install
  time, but the named volume mounted on `/var/lib/mysql` hides them. The script
  therefore runs `mariadb-install-db` when `/var/lib/mysql/mysql` (the system
  table directory) does not exist, and skips it otherwise, like the official image.
  Running `mariadb-install-db` on every start was tested and rejected: it only
  checks that `mysql.user` exists, and it returned 0 even when part of the system
  tables (`mysql/global_priv.*`) had been deleted, so it cannot detect a damaged
  data directory.
- **Configuration in bootstrap mode.** The database, the WordPress user and the
  root password are applied with `mariadbd --bootstrap`. In this mode the server
  reads SQL from standard input, runs it in the foreground, and exits; it opens
  no TCP port and no socket, so no client can connect before the configuration is
  finished, and no temporary server has to be started in the background.
  The statements are idempotent (`CREATE ... IF NOT EXISTS`, `ALTER USER`), so they
  run on every start and keep the passwords in line with the secrets.
- **Validation of names and passwords.** `DB_NAME` may have at most 64 characters
  (MariaDB limit for database names). `DB_USER` may have at most 32 characters:
  older MariaDB versions limit user names to 32 characters while newer ones allow
  more, so the lower limit is used because the MariaDB version is not pinned.
  Both passwords must be 16 to 128 characters long and contain a lowercase
  letter, an uppercase letter and a digit.
- **Network and logs.** `conf/60-inception.cnf` sets `bind-address = 0.0.0.0` so
  that MariaDB accepts connections on every interface of the container (the
  default only listens on localhost, which the WordPress container cannot reach).
  `--console` sends the server log to the container output, so it appears in
  `docker compose logs`.
- **Service user.** `mariadbd` runs as the `mysql` user created by the package.

#### WordPress + php-fpm

- WordPress is a set of PHP files that generate the HTML pages. `php-fpm` is the
  long-running process that receives PHP requests from NGINX over FastCGI
  (port 9000) and manages the PHP worker processes. `-F` keeps php-fpm in the
  foreground so that it can be PID 1.
- **WP-CLI is pinned** (`ARG WP_CLI_VER=2.12.0`, latest stable release at the time
  of writing) because it is downloaded directly from GitHub, not from `apt`.
  `curl -fsSL` fails on HTTP errors (`-f`), hides the progress bar but still shows
  errors (`-sS`), and follows redirects (`-L`).
- **Setup steps (`wp_setup.sh`).**
  1. Check the environment variables and that the database password secret is
     readable by `www-data` (the php-fpm user).
  2. Download WordPress if the core files are not in the volume yet.
  3. Wait for MariaDB (at most 30 attempts, one per second) and print the last
     connection error if it never becomes ready.
  4. Create `wp-config.php`. The file is first written as `wp-config.php.pending`
     and renamed only after `DB_PASSWORD` is set, so an interrupted start never
     leaves an incomplete `wp-config.php`. If `wp-config.php` already exists, the
     script checks that `DB_NAME`, `DB_USER` and `DB_HOST` match the environment.
  5. Install WordPress (checked with `wp core is-installed`) and create the second
     user. User names are compared without case, and a password is read from the
     secret only when that user actually has to be created.
- **The database password is never copied into a file or a command line.**
  `wp-config.php` reads `/run/secrets/db_password` with PHP at runtime, and the
  connection check also reads the secret from PHP instead of receiving it from
  the shell.

#### NGINX

- **TLSv1.2 and TLSv1.3 only** (`ssl_protocols TLSv1.2 TLSv1.3;`). TLSv1.0 and
  TLSv1.1 are deprecated because of known weaknesses (RFC 8996). Allowing both 1.2
  and 1.3 keeps older clients working, while modern clients automatically choose 1.3.
- **Self-signed certificate created when the container starts.** `42.fr` is not a
  domain we own, so no public certificate authority can issue a certificate for
  `haito.42.fr`. The certificate is generated by `nginx_setup.sh` at start-up
  rather than at build time or stored in the repository: a key created during the
  build would be stored in an image layer, where anyone with the image could
  extract it, and a key in git would break the rule that no credentials are
  committed. If a certificate already exists in the container, it is reused.
  The browser warning is expected; the traffic is still encrypted.
- **No `server_name`.** There is a single `server` block on port 443, so NGINX uses
  it for every request whatever the host name; writing the domain would not
  change the behaviour. Leaving it out keeps `DOMAIN_NAME` in `srcs/.env` as the
  only place where the domain is written.
- **Access to `/wp-config.php` is denied.** The file contains the database name and
  user and the keys used to sign WordPress log-in cookies. It is normally executed
  by PHP and prints nothing, but if PHP handling were ever misconfigured, NGINX
  could send its source as plain text. The `deny all` rule is a second line of
  defence against that.
- NGINX mounts the WordPress volume **read-only**: it only serves static files and
  passes `.php` requests to `wordpress:9000` over FastCGI.

#### Restart policy: `unless-stopped`

`on-failure` only restarts a container whose main process exits with a non-zero
status. During testing, a MariaDB server stopped from inside the container with
`docker compose -f srcs/docker-compose.yml exec mariadb mariadb-admin -u root -p shutdown`
exited with status 0, and `on-failure` left the container stopped, so the website
lost its database. With `unless-stopped`, the container is restarted whatever the
exit status, both after a crash (tested by sending `SIGKILL` to the main process
of each service) and after a clean exit. A container stopped on purpose with
`docker stop` or `make stop` stays stopped, and running containers are started
again when the Docker daemon restarts.

#### Volumes: named volumes with `driver_opts`

The subject requires named volumes whose data ends up in `/home/haito/data`, and
says that bind mounts are not allowed for them.

The volumes are declared as named volumes with the `local` driver and
`driver_opts` (`type: none`, `o: bind`, `device: /home/haito/data/<service>`).

What this does, as facts:

- In Compose and for the Docker CLI they are named volumes: they are created,
  listed, inspected and removed with `docker volume` commands, and `docker inspect`
  on a container reports the mount type `volume`.
- Underneath, the `local` driver performs a Linux bind mount: `o: bind` makes it
  bind-mount the `device` directory onto the volume's mount point.
- `docker volume inspect` shows `/home/haito/data/<service>` in `Options.device`.

How the subject is interpreted here: "bind mounts" is read as Docker's bind mount
type, i.e. mounting a host path directly into a service (`type: bind`, or
`- /home/haito/data/mariadb:/var/lib/mysql`), which this project does not do. The
subject's sentence "They remain Docker named volumes, not bind mounts" supports
this reading, but it does not settle whether a volume driver that bind-mounts
internally is acceptable. **This interpretation should be confirmed with the
evaluation sheet or the staff.**

Other ways to get the data into `/home/haito/data` were considered:

| Option | Why it was not used |
|---|---|
| Named volume without `driver_opts` | Data is stored under `/var/lib/docker/volumes`, not in `/home/haito/data`. |
| Changing Docker's `data-root` to `/home/haito/data` | The only option that uses no bind mount at any level. Not chosen because it depends on the machine's `/etc/docker/daemon.json` (needs root and a daemon restart, cannot be done from the repository) and moves every image and container of the machine there. If the staff do not accept `o: bind`, this is the fallback. |
| A separate filesystem (for example a loop device) mounted by the volume driver | Needs root and manual setup on the host. |
| A third-party volume plugin | Brings in ready-made software, against the spirit of the subject. |

#### WordPress users and the log-in link

- The stack has no mail server. WordPress self-registration sends the new user a
  link by e-mail to set the password, so registered users could never log in.
  Registration is therefore left disabled, and new users are added by an
  administrator from the administration panel, where the password is set directly.
- The default theme (Twenty Twenty-Five) shows no log-in link. `wp_setup.sh`
  publishes a navigation menu called "Header navigation" containing the page list
  and a Log in/Log out block. The header navigation of the theme has no menu
  assigned, so WordPress displays the most recently published menu, which puts the
  link in the header. The menu is created only once.

### Virtual Machines vs Docker

Both isolate applications from each other and from the host, but at different levels.

| | Virtual machine | Docker container |
|---|---|---|
| What is virtualised | Hardware: a hypervisor runs a complete guest OS with its own kernel | The operating system: containers share the host's kernel and are isolated with namespaces (what a process can see) and cgroups (what it can use) |
| Start-up | Boots a whole OS (tens of seconds) | Starts a process (about a second) |
| Size | Gigabytes (full OS image) | Megabytes to hundreds of megabytes (only the user space needed) |
| Overhead | Hardware emulation and a second kernel | Close to a normal process |
| Isolation | Stronger: a separate kernel | Weaker: a kernel bug affects every container |

In this project both are used: the subject requires a VM, which provides an
isolated machine, and Docker runs each service inside that VM in a light,
reproducible container.

### Secrets vs Environment Variables

| | Environment variables | Docker secrets |
|---|---|---|
| How they are passed | Set in the environment of the container's processes | Mounted as read-only files under `/run/secrets/<name>` |
| Who can read them | Anyone who can run `docker inspect`; also inherited by every child process and sometimes printed in logs or error pages | Only containers that list the secret in `docker-compose.yml`; the content is not shown by `docker inspect` |
| Suitable for | Non-sensitive settings | Passwords and keys |

In this project, `srcs/.env` only holds non-secret settings (names, domain,
e-mail addresses). The three passwords are files in `secrets/`, given only to the
services that need them (`db_root_password` to mariadb only, `credentials` to
wordpress only). The scripts and `wp-config.php` read them from `/run/secrets/`
when needed, so no password appears in an environment variable, a Dockerfile, a
command line or git.

### Docker Network vs Host Network

| | Docker (bridge) network | Host network |
|---|---|---|
| Network stack | Each container has its own network namespace and IP address on a virtual network | The container shares the host's network stack |
| Name resolution | Containers reach each other by service name (`mariadb`, `wordpress`) through Docker's DNS | No service-name resolution; services use `localhost` and must not use the same ports |
| Exposure | Nothing is reachable from outside unless published with `ports:` | Every port a service listens on is open on the host |

This project uses one bridge network, `inception_net`. Only NGINX publishes a port
(443); MariaDB (3306) and php-fpm (9000) are reachable only from the other
containers. With `network: host` (forbidden by the subject), MariaDB and php-fpm
would listen directly on the host and NGINX would no longer be the only entry point.

### Docker Volumes vs Bind Mounts

Both keep data outside the container, so it survives when the container is
removed. The difference is who manages the storage.

| | Docker volume | Bind mount |
|---|---|---|
| Managed by | Docker: created, listed, inspected and removed with `docker volume` | Nobody: a host path is shown inside the container as it is |
| Referenced by | A name (`mariadb_data`) | An absolute host path |
| Location | Chosen by the volume driver (default `/var/lib/docker/volumes`, configurable with `driver_opts`) | Any path on the host |
| Dependence on the host | Low: the compose file only names the volume | High: depends on the host's directory layout and permissions |
| Empty volume first mounted | Docker copies the image's files at the mount point into it | The host directory hides the image's files |

This project uses two named volumes; see "Volumes" in the design choices above for
how their data is placed in `/home/haito/data`.

## Resources

Official images, used as references for the setup scripts and configuration files:

- [MariaDB image entrypoint](https://github.com/MariaDB/mariadb-docker/blob/master/docker-entrypoint.sh) - reference for `db_setup.sh`
- [NGINX official image](https://github.com/nginx/docker-nginx)
- [WordPress official image](https://github.com/docker-library/wordpress)

Official documentation, used for directory layouts, options and behaviour:

- [Debian releases](https://www.debian.org/releases/) - used to choose the base image
- [MariaDB documentation](https://mariadb.com/kb/en/documentation/) - `mariadb-install-db`, `mariadbd` options, users and privileges
- [NGINX documentation](https://nginx.org/en/docs/) - `ngx_http_ssl_module`, `ngx_http_fastcgi_module`
- [WordPress developer resources](https://developer.wordpress.org/) and [WP-CLI commands](https://developer.wordpress.org/cli/commands/)
- [Docker documentation](https://docs.docker.com/) - Dockerfile, Compose file reference, volumes, secrets, networking

Other:

- [Video on digital certificates and encrypted communication (YouTube)](https://youtu.be/S5jJJAGC5vs) - used to learn how certificates and TLS work

### Use of AI

AI (Claude) was used as an assistant for:

- **Design discussion**: comparing approaches before choosing one, for example
  how to place named volumes in `/home/haito/data`, how to run the MariaDB initial
  configuration in the foreground (`mariadbd --bootstrap`), splitting the
  Dockerfiles into `ENTRYPOINT` (setup script) and `CMD` (main process), and the
  restart policy.
- **Debugging**: reading container logs to find configuration errors (for example
  typos in `nginx.conf`) and failing password checks.
- **Reviewing against the requirements**: checking the project against the subject
  and the evaluation sheet, and running end-to-end tests (TLS versions, port 80
  closed, crash and restart, persistence after `make down`).
- **Information gathering and documentation**: looking up how Docker, MariaDB,
  NGINX and WordPress behave, and translating and editing these documents from my
  own notes.

Every suggestion was tested in the running stack before being kept, and I made
the design decisions myself.
