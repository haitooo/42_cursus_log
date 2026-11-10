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

## Bonus services

### What they provide

| Service | Role | Access |
|---|---|---|
| `redis` | Object cache for WordPress: database results are kept in memory and reused | Inside the Docker network only |
| `ftp` | vsftpd giving access to the WordPress files | `ftp://127.0.0.1` (port 21, passive ports 21100-21110), from the machine itself only |
| `adminer` | Web interface to browse and query the database | http://localhost:8082, from the machine itself only |
| `website` | Static page (HTML and CSS) | http://localhost:8081 |
| `mail` | Postfix relay that sends WordPress e-mails through Gmail. It also enables the **Register** link on the log-in page | Inside the Docker network only |

### Start and stop

```sh
make bonus    # mandatory services + bonus services
make ps       # all eight services must show "Up"
make down     # stops everything, bonus services included
make          # back to the mandatory services only
```

With `make` only, Redis, e-mail sending and self-registration are switched off
again automatically.

The checks below use this shortcut (run from the repository root; the alias only
exists in the terminal where it was defined):

```sh
alias incb='docker compose -f srcs/docker-compose.yml -f src/docker-compose.bonus.yml'
```

### Check the static website

Open http://localhost:8081. The page shows the "INCEPTION" ASCII art.
Requests are logged by `incb logs -f website`.

### Check the Redis cache

1. The cache is connected:
   ```sh
   incb exec wordpress wp --allow-root redis status    # Status: Connected
   ```
2. Pages are served from the cache. Close any tab with the administration panel
   open (it sends background requests), then:
   ```sh
   incb exec redis redis-cli config resetstat         # reset the counters
   # open https://haito.42.fr once in the browser
   incb exec redis redis-cli info stats | grep keyspace
   ```
   `keyspace_hits` increases (about 35 for the home page) and `keyspace_misses`
   stays at 0: the data was found in Redis.
3. Compare with an empty cache:
   ```sh
   incb exec redis redis-cli flushall                 # empty the cache
   incb exec redis redis-cli config resetstat
   # open the page once
   incb exec redis redis-cli info stats | grep keyspace
   ```
   This time `keyspace_hits` is 0 and `keyspace_misses` increases (about 44):
   the data was read from the database and stored in Redis. Opening the page
   again gives hits and no misses.
4. To watch the commands live, run `incb exec redis redis-cli monitor` and open
   pages in the browser (Ctrl+C to stop).

Clearing the cache: `incb exec wordpress wp --allow-root cache flush`, or
**Settings > Redis > Flush Cache** in the administration panel. Only cached
copies are removed; the data stays in the database.

The comment at the end of each page's HTML ("Retrieved N objects ... from
Redis") shows that the Redis object cache is active, but N counts every cache
lookup during the request, including the ones answered from PHP memory. It
barely changes after a flush, so use the `keyspace_hits` / `keyspace_misses`
counters above to check the cache itself.

### Check the FTP server

Replace `<user>` with `FTP_USER` from `srcs/.env`; the password is in
`secrets/ftp_password.txt`.

```sh
curl --user <user> ftp://127.0.0.1/                                   # list the WordPress files
echo "hello from ftp" > hello.txt
curl --user <user> -T hello.txt ftp://127.0.0.1/wp-content/uploads/   # upload
curl --user <user> ftp://127.0.0.1/wp-content/uploads/hello.txt       # download
```

- The uploaded file is also served by the website:
  https://haito.42.fr/wp-content/uploads/hello.txt
- Writing next to the WordPress core files is refused (`553`), a wrong password
  or an anonymous login is refused (`530`), and the user cannot leave the
  WordPress directory.
- Log-ins and transfers: `incb exec ftp cat /var/log/vsftpd.log`

### Check Adminer

Open http://localhost:8082 and log in with:

| Field | Value |
|---|---|
| System | MySQL |
| Server | `mariadb` |
| Username | `DB_USER` from `srcs/.env` |
| Password | content of `secrets/db_password.txt` |
| Database | `DB_NAME` from `srcs/.env` |

The tables `wp_users`, `wp_posts`, ... are listed on the left. **SQL command**
runs a query, for example `SELECT user_login FROM wp_users;`.

### Check e-mail sending and self-registration

1. Open https://haito.42.fr/wp-login.php and click **Register**.
2. Enter a user name and an e-mail address you can read.
3. An e-mail with a link to set the password arrives (check the spam folder).
4. Follow the relay in the logs:
   ```sh
   incb logs -f mail
   ```
   `status=sent` means Gmail accepted the message; `status=deferred` is followed
   by the reason (for example a rejected app password).

A single test message can also be sent from the command line:

```sh
incb exec wordpress wp --allow-root eval 'var_dump(wp_mail("<your address>", "Inception test", "hello"));'
```

"Lost your password?" on the log-in page uses the same path.
