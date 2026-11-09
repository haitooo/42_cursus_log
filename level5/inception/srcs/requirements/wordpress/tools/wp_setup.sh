#!/bin/sh
set -e

WP_DIR="/var/www/html"
DB_HOST="mariadb:3306"
DB_PASS_PATH="/run/secrets/db_password"
CREDENTIALS_PATH="/run/secrets/credentials"
FPM_USER="www-data"

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
RESET='\033[0m'

log_info() {
	printf "${BLUE}[INFO]${RESET} %s\n" "$1"
}

log_success() {
	printf "${GREEN}[ OK ]${RESET} %s\n" "$1"
}

log_error() {
	printf "${RED}[ERROR]${RESET} %s\n" "$1" >&2
}

wp_cli() {
	wp --allow-root --path="$WP_DIR" "$@"
}

validate_env() {
	log_info "Validating environment variables..."

	if test -z "${DB_NAME}"; then
		log_error "DB_NAME is required."
		exit 1
	fi

	if test -z "${DB_USER}"; then
		log_error "DB_USER is required."
		exit 1
	fi

	if test -z "${DOMAIN_NAME}"; then
		log_error "DOMAIN_NAME is required."
		exit 1
	fi

	if test -z "${WP_TITLE}"; then
		log_error "WP_TITLE is required."
		exit 1
	fi

	if test -z "${WP_ADMIN_USER}"; then
		log_error "WP_ADMIN_USER is required."
		exit 1
	fi

	if test -z "${WP_ADMIN_EMAIL}"; then
		log_error "WP_ADMIN_EMAIL is required."
		exit 1
	fi

	if test -z "${WP_USER}"; then
		log_error "WP_USER is required."
		exit 1
	fi

	if test -z "${WP_USER_EMAIL}"; then
		log_error "WP_USER_EMAIL is required."
		exit 1
	fi

	admin_login=$(printf '%s' "$WP_ADMIN_USER" | tr '[:upper:]' '[:lower:]')
	user_login=$(printf '%s' "$WP_USER" | tr '[:upper:]' '[:lower:]')

	case "$admin_login" in
		*admin*)
			log_error "WP_ADMIN_USER must not contain 'admin'."
			exit 1
			;;
	esac

	if test "$admin_login" = "$user_login"; then
		log_error "WP_ADMIN_USER and WP_USER must differ, ignoring case."
		exit 1
	fi

	log_success "WordPress configuration validated."
}

read_credential() {
	if test ! -f "$CREDENTIALS_PATH" || test ! -r "$CREDENTIALS_PATH"; then
		log_error "Credentials secret is missing or unreadable."
		exit 1
	fi

	credential=$(sed -n "s/^$1=//p" "$CREDENTIALS_PATH" | head -n 1)

	if test -z "$credential"; then
		log_error "$1 is missing in credentials secret."
		exit 1
	fi

	printf '%s\n' "$credential"
}

validate_database_secret() {
	log_info "Validating database password secret..."

	if test ! -f "$DB_PASS_PATH" || test ! -r "$DB_PASS_PATH"; then
		log_error "Database password secret is missing or unreadable."
		exit 1
	fi

	if ! su -s /bin/sh -c "test -r '$DB_PASS_PATH'" "$FPM_USER"; then
		log_error "Database password secret is not readable by $FPM_USER."
		exit 1
	fi

	log_success "Database password secret validated."
}

download_wordpress() {
	if test -f "$WP_DIR/wp-includes/version.php"; then
		log_info "WordPress core files already exist. Skipping download."
		return 0
	fi

	log_info "Downloading WordPress core files..."

	if ! wp_cli core download; then
		log_error "WordPress download failed."
		exit 1
	fi

	log_success "WordPress core files downloaded."
}

check_db_connection() {
	DB_HOST="$DB_HOST" DB_NAME="$DB_NAME" DB_USER="$DB_USER" \
	DB_PASS_PATH="$DB_PASS_PATH" \
	php -r '
		mysqli_report(MYSQLI_REPORT_OFF);
		$password = file_get_contents(getenv("DB_PASS_PATH"));
		if ($password === false) {
			fwrite(STDERR, "Could not read database password secret.\n");
			exit(1);
		}
		$password = rtrim($password, "\n");
		[$host, $port] = explode(":", getenv("DB_HOST"));
		$db = mysqli_init();
		$db->options(MYSQLI_OPT_CONNECT_TIMEOUT, 3);
		$connected = @$db->real_connect($host, getenv("DB_USER"), $password,
			getenv("DB_NAME"), (int)$port);
		if (!$connected) {
			fwrite(STDERR, mysqli_connect_error() . "\n");
			exit(1);
		}
		$db->close();
	'
}

wait_for_database() {
	log_info "Waiting for MariaDB at $DB_HOST..."

	attempt=0
	until db_error=$(check_db_connection 2>&1)
	do
		attempt=$((attempt + 1))

		if test "$attempt" -ge 30; then
			printf '\n'
			log_error "MariaDB did not become ready after 30 attempts: $db_error"
			exit 1
		fi

		printf "\r${YELLOW}[WAIT]${RESET} Waiting for MariaDB... (%d/30)" "$attempt"
		sleep 1
	done

	if test "$attempt" -gt 0; then
		printf '\n'
	fi

	log_success "Connected to MariaDB as $DB_USER."
}

create_config() {
	if test -f "$WP_DIR/wp-config.php"; then
		log_info "wp-config.php already exists. Skipping creation."
		check_config
		return 0
	fi

	log_info "Creating wp-config.php..."
	config_path="$WP_DIR/wp-config.php.pending"

	(
		umask 077
		wp_cli config create \
			--dbname="$DB_NAME" \
			--dbuser="$DB_USER" \
			--dbhost="$DB_HOST" \
			--config-file="$config_path" \
			--force --skip-check --quiet
	)

	wp_cli config set DB_PASSWORD \
		"rtrim(file_get_contents('$DB_PASS_PATH'), \"\\n\")" \
		--config-file="$config_path" --raw --quiet

	mv "$config_path" "$WP_DIR/wp-config.php"

	log_success "wp-config.php created."
}

check_config_value() {
	config_key="$1"
	expected_value="$2"
	actual_value=$(wp_cli config get "$config_key")

	if test "$actual_value" != "$expected_value"; then
		log_error "wp-config.php has $config_key='$actual_value' but the environment has '$expected_value'."
		log_error "Fix .env or remove wp-config.php from the volume."
		exit 1
	fi
}

check_config() {
	log_info "Checking wp-config.php against environment variables..."

	check_config_value DB_NAME "$DB_NAME"
	check_config_value DB_USER "$DB_USER"
	check_config_value DB_HOST "$DB_HOST"

	log_success "wp-config.php matches environment variables."
}

install_wordpress() {
	if install_output=$(wp_cli core is-installed --url="https://$DOMAIN_NAME" 2>&1); then
		log_info "WordPress is already installed. Skipping installation."
		return 0
	else
		install_status=$?
		if test "$install_status" -ne 1 || test -n "$install_output"; then
			log_error "Could not check WordPress install state (exit $install_status): $install_output"
			exit 1
		fi
	fi

	log_info "Installing WordPress..."
	WP_ADMIN_PASSWORD=$(read_credential WP_ADMIN_PASSWORD)

	printf '%s\n' "$WP_ADMIN_PASSWORD" | wp_cli core install \
		--url="https://$DOMAIN_NAME" \
		--title="$WP_TITLE" \
		--admin_user="$WP_ADMIN_USER" \
		--admin_email="$WP_ADMIN_EMAIL" \
		--prompt=admin_password \
		--skip-email >/dev/null

	log_success "WordPress installed with administrator $WP_ADMIN_USER."
}

create_second_user() {
	existing=$(wp_cli user list --login="$WP_USER" --field=user_login)

	if test -n "$existing"; then
		log_info "User $WP_USER already exists. Skipping creation."
		return 0
	fi

	log_info "Creating user $WP_USER..."
	WP_USER_PASSWORD=$(read_credential WP_USER_PASSWORD)

	printf '%s\n' "$WP_USER_PASSWORD" | wp_cli user create \
		"$WP_USER" "$WP_USER_EMAIL" \
		--role=author \
		--prompt=user_pass >/dev/null

	log_success "User $WP_USER created."
}

set_permissions() {
	log_info "Setting permissions for $FPM_USER..."

	chown "root:$FPM_USER" "$WP_DIR/wp-config.php"
	chmod 640 "$WP_DIR/wp-config.php"

	mkdir -p "$WP_DIR/wp-content/uploads"
	chown "$FPM_USER:$FPM_USER" "$WP_DIR/wp-content/uploads"

	log_success "Permissions set."
}

main() {
	log_info "Starting WordPress setup..."

	validate_env
	validate_database_secret

	download_wordpress
	wait_for_database
	create_config
	install_wordpress
	create_second_user
	set_permissions

	log_success "WordPress setup completed."
}

main
