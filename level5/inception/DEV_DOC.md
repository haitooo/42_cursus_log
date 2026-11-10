# Developer documentation

## Set up the environment from scratch

### Prerequisites

- A Linux virtual machine (the project was developed on Debian/Ubuntu).
- Docker Engine with the Compose plugin (`docker compose`, not the old
  `docker-compose`). Tested with Docker 29.1.3 and Docker Compose v5.0.0.
- `make`.
- A user that can run `docker` without `sudo` (member of the `docker` group).
- Write access to `/home/haito`, because `make` creates `/home/haito/data`.
- A web browser on the same machine, to open https://haito.42.fr.

Only `debian:bookworm` is pulled from Docker Hub. Every other image is built from
the Dockerfiles in `srcs/requirements/`.

### Domain name

`haito.42.fr` must resolve to the local machine. Add this line to `/etc/hosts`
(root privileges required):

```
127.0.0.1 haito.42.fr
```

### Configuration files

Copy the example and fill in every value:

```sh
cp srcs/.env.example srcs/.env
```

`srcs/.env` is ignored by git. Docker Compose passes it to every container
(`env_file: .env`).

| Variable | Used by | Meaning and rules |
|---|---|---|
| `DB_NAME` | mariadb, wordpress | Name of the WordPress database. Letters, digits and `_` only, at most 64 characters. |
| `DB_USER` | mariadb, wordpress | MariaDB user used by WordPress. Letters, digits and `_` only, at most 32 characters, must not be `root`. |
| `DOMAIN_NAME` | wordpress, nginx | Site address (`haito.42.fr`). WordPress uses it for the site URL, NGINX for the certificate. Letters, digits, `.` and `-` only. |
| `WP_TITLE` | wordpress | Site title. |
| `WP_ADMIN_USER` | wordpress | Administrator user name. Must not contain `admin` in any letter case. |
| `WP_ADMIN_EMAIL` | wordpress | Administrator e-mail address. |
| `WP_USER` | wordpress | Second user (role: author). Must differ from `WP_ADMIN_USER`, ignoring case. |
| `WP_USER_EMAIL` | wordpress | Second user's e-mail address. |
| `FTP_USER` | ftp (bonus) | FTP user name. Lowercase letters, digits, `_` and `-` only, at most 32 characters, must not be an existing system user. |
| `MAIL_RELAY_HOST` | mail (bonus) | SMTP server that delivers the e-mails: `smtp.gmail.com`. |
| `MAIL_RELAY_PORT` | mail (bonus) | Port of the SMTP server: `587` (STARTTLS). |
| `MAIL_RELAY_USER` | mail (bonus) | Gmail address used to log in to the SMTP server. |

The four bonus variables are only needed for `make bonus`.

The setup scripts check these rules at start-up and stop with an `[ERROR]` line if
a value is missing or invalid.

### Secrets

Create the three files in `secrets/` (the whole directory content is ignored by git):

| File | Format | Used by |
|---|---|---|
| `secrets/db_root_password.txt` | The password on one line | mariadb |
| `secrets/db_password.txt` | The password on one line | mariadb, wordpress |
| `secrets/credentials.txt` | Two lines: `WP_ADMIN_PASSWORD=...` and `WP_USER_PASSWORD=...` | wordpress |
| `secrets/ftp_password.txt` (bonus) | The FTP password on one line: 8 to 128 characters, letters, digits and `_ ! @ # % + = ? -` | ftp |
| `secrets/mail_password.txt` (bonus) | A Gmail app password (16 letters, spaces allowed as shown by Google) | mail |

A Gmail app password is created at https://myaccount.google.com/apppasswords
after turning on 2-Step Verification. It is not the account password, and it can
be revoked on its own.

Rules checked by `db_setup.sh` for both database passwords:

- 8 to 128 characters;
- only letters, digits and `_ ! @ # % + = ? -`;
- at least one lowercase letter, one uppercase letter and one digit.

Docker Compose mounts each file read-only under `/run/secrets/<name>` in the
containers that list it. The files keep their host permissions inside the
container, and php-fpm (user `www-data`) must be able to read
`db_password`, so keep the files readable (mode `644`, as created by default).
`wp_setup.sh` stops with `Database password secret is not readable by www-data.`
otherwise.

`make` refuses to start if `srcs/.env` or one of the three secret files is missing;
`make bonus` also requires the two bonus secret files.

## Build and launch the project

```sh
make
```

`make` (target `all`, same as `make up`) does the following:

1. `check_files`: checks that `srcs/.env` and the three secret files exist.
2. Creates `/home/haito/data/mariadb` and `/home/haito/data/wordpress` if they do
   not exist (the volumes point to these directories).
3. Runs `docker compose -f srcs/docker-compose.yml up -d --build`: builds the three
   images (`mariadb:inception`, `wordpress:inception`, `nginx:inception`) and
   starts the containers in the background.

On the first start the containers then:

- `mariadb`: initialises the data directory and creates the database, the
  WordPress user and the root password;
- `wordpress`: downloads WordPress, waits for MariaDB, writes `wp-config.php`,
  installs the site and creates the two users;
- `nginx`: creates a self-signed certificate for `DOMAIN_NAME`.

The equivalent commands without `make`:

```sh
mkdir -p /home/haito/data/mariadb /home/haito/data/wordpress
docker compose -f srcs/docker-compose.yml build
docker compose -f srcs/docker-compose.yml up -d
```

To follow the setup in the foreground instead, run
`docker compose -f srcs/docker-compose.yml up --build` (stop with Ctrl+C).
`docker compose -f srcs/docker-compose.yml build --progress=plain --no-cache`
shows the full output of every build step.

### Bonus services

```sh
make bonus
```

`make bonus` runs the same checks, also checks the two bonus secret files, and
starts the stack with a second Compose file:

```sh
docker compose -f srcs/docker-compose.yml -f srcs/docker-compose.bonus.yml up -d --build
```

`srcs/docker-compose.bonus.yml` adds the services `redis`, `ftp`, `adminer`,
`website` and `mail` (their Dockerfiles are in `srcs/requirements/bonus/`), and
gives the `wordpress` service two extra variables, `REDIS_HOST=redis` and
`MAIL_HOST=mail`. On start, `wp_setup.sh` calls `wp_bonus_setup.sh` twice
(`before-install`, before WordPress is loaded, and `after-install`), which uses
them to:

- install and enable the Redis Object Cache plugin and its drop-in
  (`wp-content/object-cache.php`);
- write a must-use plugin (`wp-content/mu-plugins/inception-smtp.php`) that sends
  e-mails over SMTP to `mail:25`, and turn on self-registration.

When the stack is started again with `make`, the variables are absent and
`wp_bonus_setup.sh` removes the drop-in and the must-use plugin and turns
self-registration off, before WordPress is loaded. The mandatory services
therefore never depend on a bonus container.

| Service | Base | Published ports | Notes |
|---|---|---|---|
| `redis` | `redis-server` (bookworm) | none | Cache only: no persistence, 64 MB limit, LRU eviction |
| `ftp` | `vsftpd` | `127.0.0.1:21`, `127.0.0.1:21100-21110` (passive mode) | Shares `wordpress_data`; the FTP user has the UID/GID of `www-data` and is locked in `/var/www/html`; log in `/var/log/vsftpd.log` (vsftpd cannot log to stdout) |
| `adminer` | `adminer` package, served by `php -S` | `127.0.0.1:8082` → 8080 | Runs as `www-data` |
| `website` | `busybox httpd` | `8081` | Static HTML and CSS in `srcs/requirements/bonus/website/site/` |
| `mail` | Postfix | none | Relays to `MAIL_RELAY_HOST:MAIL_RELAY_PORT` with TLS and SASL; accepts mail only from private networks |

Adminer and FTP are bound to `127.0.0.1` because they give direct access to the
database and the files: they are usable from the VM only.

## Manage the containers and volumes

### Makefile targets

`COMPOSE` uses `srcs/docker-compose.yml`; `COMPOSE_ALL` uses it together with
`srcs/docker-compose.bonus.yml`, so that the management targets also act on the
bonus containers whether they were started or not.

| Target | Command run |
|---|---|
| `all`, `up` | `check_files`, create the data directories, `$(COMPOSE) up -d --build` |
| `bonus` | `check_files`, `check_bonus_files`, create the data directories, `$(COMPOSE_ALL) up -d --build` |
| `build` | `check_files`, `$(COMPOSE) build` |
| `down` | `$(COMPOSE_ALL) down` |
| `stop` / `start` | `$(COMPOSE_ALL) stop` / `$(COMPOSE_ALL) start` |
| `ps` | `$(COMPOSE_ALL) ps` |
| `logs` | `$(COMPOSE_ALL) logs -f` |
| `mariadb` / `nginx` / `wordpress` | `$(COMPOSE) exec <service> bash`: open a shell in that container |
| `clean` | `$(COMPOSE_ALL) down --volumes --rmi all --remove-orphans` |
| `fclean` | `clean`, then deletes `/home/haito/data/mariadb` and `/home/haito/data/wordpress` from a `debian:bookworm` container (the files belong to container users such as `mysql`, so the host user cannot delete them without `sudo`) |
| `re` | `fclean`, then `all` |

### Useful commands

All `docker compose` commands below are run from the repository root. For the
bonus services, add `-f srcs/docker-compose.bonus.yml` after the first `-f`
option, or define a shortcut:

```sh
alias incb='docker compose -f srcs/docker-compose.yml -f srcs/docker-compose.bonus.yml'
```

```sh
# Logs of one service (add -f to follow, -t for timestamps)
docker compose -f srcs/docker-compose.yml logs wordpress

# Shell inside a running container
docker compose -f srcs/docker-compose.yml exec wordpress sh

# WP-CLI inside the wordpress container
docker compose -f srcs/docker-compose.yml exec wordpress wp --allow-root user list

# Rebuild and restart one service
docker compose -f srcs/docker-compose.yml up -d --build nginx

# Volumes and network
docker volume ls
docker volume inspect inception_mariadb_data inception_wordpress_data
docker network ls
docker network inspect inception_inception_net
```

Logs are kept while the container exists, including logs from before an automatic
restart. `make down` and `make clean` remove the containers and their logs;
`make stop` keeps them.

## Where the data is stored and how it persists

| Volume | Mounted at | Data on the host |
|---|---|---|
| `inception_mariadb_data` | `/var/lib/mysql` in `mariadb` | `/home/haito/data/mariadb` |
| `inception_wordpress_data` | `/var/www/html` in `wordpress` (read-write) and `nginx` (read-only) | `/home/haito/data/wordpress` |

Both are Docker named volumes using the `local` driver with
`driver_opts: { type: none, o: bind, device: /home/haito/data/... }`, so their
content is written directly to the directories above.
`docker volume inspect` shows the host path in `Options.device`.

What each command does to the data:

| Command | Containers | Volumes (Docker objects) | Files in `/home/haito/data` |
|---|---|---|---|
| `make stop` | Stopped | Kept | Kept |
| `make down` | Removed | Kept | Kept |
| `make clean` | Removed | Removed | **Kept** |
| `make fclean` | Removed | Removed | Deleted |

Because the files stay in `/home/haito/data`, the next `make` reuses them. The
setup scripts detect what already exists and skip it: MariaDB is not initialised
again, WordPress is not downloaded or installed again, and existing users are not
recreated. Posts, comments, uploads and settings therefore survive restarts,
`make down`, `make clean` and reboots of the machine.

The TLS certificate is stored inside the `nginx` container, not in a volume. A new
one is generated whenever the `nginx` container is recreated.

Bonus services add no volume of their own: `ftp` works on `wordpress_data`, the
Redis cache is kept in memory only (it is rebuilt from the database after a
restart), and messages waiting in the Postfix queue are lost when the `mail`
container is recreated.

## Troubleshooting MariaDB start-up

### How the setup script decides to initialise

`db_setup.sh` checks whether `/var/lib/mysql/mysql` (the system table directory)
exists in the volume:

- missing: it runs `mariadb-install-db` to create a new data directory;
- present: it treats the data directory as initialised and skips that step,
  like the official MariaDB image.

The directory can exist and still be damaged. That is detected in the next step,
when `mariadbd --bootstrap` has to open the data directory to apply the
configuration.

### Error messages

If the data directory is damaged, the MariaDB container stops with one of these
messages (see `docker compose -f srcs/docker-compose.yml logs mariadb`):

- `[ERROR] MariaDB initialization failed. Inspect /var/lib/mysql manually.`
  - `mariadb-install-db` failed.
- `[ERROR] Failed to configure the database in bootstrap mode.`
  - `mariadbd --bootstrap` could not open the data directory or run the SQL,
    for example because files under `/var/lib/mysql/mysql` are missing or corrupted.
    The MariaDB error follows it in the log, for example
    `Fatal error: Can't open and lock privilege tables: 'mysql.user' is not of type 'TABLE'`
    when `mysql/global_priv.*` has been deleted.

### Recovering by recreating the data directory

The setup script never deletes data by itself, so that a healthy database is
never wiped by mistake. Once the logs confirm that the data directory is damaged:

1. Stop the containers: `make down`
2. Delete the MariaDB data on the host. The files belong to the container's
   `mysql` user, so they are removed from a container instead of with `sudo`:
   ```sh
   docker run --rm -v /home/haito/data:/data debian:bookworm \
       find /data/mariadb -mindepth 1 -delete
   ```
   `docker volume rm` is not enough here: the volume only points to
   `/home/haito/data/mariadb`, and removing the volume leaves the files on the host.
3. Start again: `make`

With an empty data directory, the next start runs `mariadb-install-db` again.

**Warning:** step 2 deletes the whole database, including WordPress posts and users.
Make a backup first if any data must be kept. `make fclean` also works, but it
deletes the WordPress files as well.

## Troubleshooting bonus services

- **E-mails are not delivered.** Run `incb logs mail`. `status=deferred (SASL
  authentication failed ...)` means Gmail refused the login: check
  `MAIL_RELAY_USER` and that `secrets/mail_password.txt` holds a valid app
  password (not the account password), then run `make down` and `make bonus`.
  The VM must be able to reach `smtp.gmail.com` on port 587.
- **A bonus port cannot be opened from the browser.** Another program on the host
  may already use it. On WSL, for example, a Windows `netsh interface portproxy`
  rule on port 8080 prevented the port from being forwarded, which is why
  Adminer is published on 8082. `netsh interface portproxy show all` lists such
  rules.
- **FTP lists nothing or times out after logging in.** The client must use
  passive mode; data connections go to ports 21100-21110 on `127.0.0.1`.
