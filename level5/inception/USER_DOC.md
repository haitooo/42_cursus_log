# User documentation

## Services provided by the stack

The stack runs a WordPress website at **https://haito.42.fr**. It is made of three
services, each in its own container:

| Service | Role | Reachable from the host |
|---|---|---|
| `nginx` | Web server and the only entry point. Serves HTTPS on port 443 with TLSv1.2 or TLSv1.3 and forwards PHP requests to `wordpress`. | Yes, port 443 only |
| `wordpress` | WordPress run by php-fpm. Builds the pages and talks to the database. | No (port 9000 inside the Docker network) |
| `mariadb` | MariaDB database that stores posts, pages, comments, users and settings. | No (port 3306 inside the Docker network) |

Plain HTTP (port 80) is not served.

## Start and stop the project

Run these commands from the root of the repository.

| Command | What it does | Data |
|---|---|---|
| `make` | Builds the images if needed and starts the three services in the background. | Kept |
| `make stop` | Stops the containers without removing them. | Kept |
| `make start` | Starts containers stopped with `make stop`. | Kept |
| `make down` | Stops and removes the containers and the network. | Kept |
| `make clean` | Same as `make down`, and also removes the Docker volumes and the built images. | Kept on disk in `/home/haito/data` |
| `make fclean` | Same as `make clean`, and also deletes `/home/haito/data/mariadb` and `/home/haito/data/wordpress`. | **Deleted** |
| `make re` | `make fclean` followed by `make`: starts again from an empty site. | **Deleted** |

The containers restart automatically if their main process crashes. A container
stopped on purpose (`make stop`, `docker stop`) stays stopped until it is started again.

## Access the website and the administration panel

| Page | URL |
|---|---|
| Website | https://haito.42.fr |
| Log in | https://haito.42.fr/wp-login.php, or the **Log in** link in the header |
| Administration panel | https://haito.42.fr/wp-admin/ |

The certificate is self-signed, so the browser shows a security warning the first
time. Choose "Advanced" and continue to `haito.42.fr`. The connection is still
encrypted with TLS.

`haito.42.fr` must point to the machine running the stack (see "Domain name" in
`DEV_DOC.md`).

## Locate and manage credentials

All credentials stay on the machine and are ignored by git.

| File | Contents |
|---|---|
| `srcs/.env` | Non-secret settings: database name and user name, domain name, site title, WordPress user names and e-mail addresses |
| `secrets/db_root_password.txt` | Password of the MariaDB `root` user |
| `secrets/db_password.txt` | Password of the MariaDB user used by WordPress (`DB_USER` in `srcs/.env`) |
| `secrets/credentials.txt` | Passwords of the two WordPress users: `WP_ADMIN_PASSWORD=...` and `WP_USER_PASSWORD=...` |

WordPress accounts created at the first start:

| User name | Role | Password |
|---|---|---|
| `WP_ADMIN_USER` in `srcs/.env` | Administrator | `WP_ADMIN_PASSWORD` in `secrets/credentials.txt` |
| `WP_USER` in `srcs/.env` | Author (can write posts and comments) | `WP_USER_PASSWORD` in `secrets/credentials.txt` |

### Adding a WordPress user

Visitors cannot register by themselves. An administrator adds users:

1. Log in with the administrator account and open the administration panel.
2. Go to **Users > Add New User**.
3. Fill in the user name, e-mail, password and role.
4. Uncheck **Send the new user an email about their account**: the stack has no
   mail server, so the e-mail would not be delivered.
5. Click **Add New User**. The user can log in immediately with that password.

### Changing a password after the first start

| Password | How to change it |
|---|---|
| MariaDB root (`secrets/db_root_password.txt`) | Edit the file, then run `make down` and `make`. The MariaDB setup script applies the password in the secret on every start. |
| MariaDB WordPress user (`secrets/db_password.txt`) | Edit the file, then run `make down` and `make`. MariaDB applies the new password on start, and WordPress reads the secret on every request. |
| WordPress users (`secrets/credentials.txt`) | Change it in the administration panel (Users > Profile, or Users > All Users > Edit). The secret is only read when a user is created for the first time, so editing the file later has no effect. |

`make down` is used instead of a plain restart because Docker mounts a secret file
when the container is created; recreating the containers guarantees that the new
file content is used.

## Check that the services are running correctly

1. **All three containers are up.**
   ```sh
   make ps
   ```
   `mariadb`, `wordpress` and `nginx` must show `Up`, and only `nginx` publishes a
   port (`0.0.0.0:443->443/tcp`).
2. **The setup scripts finished.**
   ```sh
   docker compose -f srcs/docker-compose.yml logs mariadb wordpress nginx | grep "setup completed"
   ```
   One `setup completed` line per service is expected. A line starting with
   `[ERROR]` explains why a service stopped.
3. **The website answers over HTTPS only.**
   Open https://haito.42.fr: the site must appear (not the WordPress installation
   page). `http://haito.42.fr` must not connect.
4. **TLS version.**
   ```sh
   openssl s_client -connect haito.42.fr:443 -tls1_2 </dev/null 2>/dev/null | grep -E 'Protocol|Cipher is'
   openssl s_client -connect haito.42.fr:443 -tls1_1 </dev/null   # must fail
   ```
5. **The database contains the WordPress data.**
   ```sh
   docker compose -f srcs/docker-compose.yml exec mariadb mariadb -u root -p
   ```
   Enter the password from `secrets/db_root_password.txt`, then:
   ```sql
   SHOW DATABASES;
   SELECT user_login FROM wordpress.wp_users;
   ```
   Replace `wordpress` with `DB_NAME` from `srcs/.env` if it is different.
