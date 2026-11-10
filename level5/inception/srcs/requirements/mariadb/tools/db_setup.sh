#!/bin/sh
set -e

DB_PASS_PATH="/run/secrets/db_password"
DB_ROOT_PASS_PATH="/run/secrets/db_root_password"
DB_DATA_DIR="/var/lib/mysql"
DB_RUNTIME_DIR="/run/mysqld"

RED='\033[0;31m'
GREEN='\033[0;32m'
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

	if test "$DB_USER" = "root"; then
		log_error "DB_USER must not be root."
		exit 1
	fi

	case "$DB_NAME" in
		*[!a-zA-Z0-9_]*)
			log_error "DB_NAME contains invalid characters."
			exit 1
			;;
	esac

	case "$DB_USER" in
		*[!a-zA-Z0-9_]*)
			log_error "DB_USER contains invalid characters."
			exit 1
			;;
	esac

	if test "${#DB_NAME}" -gt 64; then
		log_error "DB_NAME must be at most 64 characters."
		exit 1
	fi

	if test "${#DB_USER}" -gt 32; then
		log_error "DB_USER must be at most 32 characters."
		exit 1
	fi

	log_success "Database configuration validated."
}

validate_password_file() {
	if test ! -f "$1" || test ! -r "$1"; then
		log_error "$2 password secret is missing or unreadable."
		exit 1
	fi
}

validate_password_value() {
	password="$1"
	password_name="$2"

	if test "${#password}" -lt 16; then
		log_error "$password_name password must be at least 16 characters."
		exit 1
	fi

	if test "${#password}" -gt 128; then
		log_error "$password_name password must not exceed 128 characters."
		exit 1
	fi

	case "$password" in
		*[!a-zA-Z0-9_!@#%+=?-]*)
			log_error "$password_name password contains unsupported characters."
			exit 1
			;;
	esac

	case "$password" in
		*[a-z]*) ;;
		*)
			log_error "$password_name password must contain a lowercase letter."
			exit 1
			;;
	esac

	case "$password" in
		*[A-Z]*) ;;
		*)
			log_error "$password_name password must contain an uppercase letter."
			exit 1
			;;
	esac

	case "$password" in
		*[0-9]*) ;;
		*)
			log_error "$password_name password must contain a digit."
			exit 1
			;;
	esac
}

validate_passwords() {
	log_info "Validating database passwords..."

	validate_password_file "$DB_PASS_PATH" "Database"
	validate_password_file "$DB_ROOT_PASS_PATH" "Root"

	DB_PASSWORD=$(cat "$DB_PASS_PATH")
	DB_ROOT_PASSWORD=$(cat "$DB_ROOT_PASS_PATH")

	validate_password_value "$DB_PASSWORD" "Database"
	validate_password_value "$DB_ROOT_PASSWORD" "Root"

	log_success "Database passwords validated."
}

prepare_runtime() {
	log_info "Preparing MariaDB runtime directory..."

	mkdir -p "$DB_RUNTIME_DIR"
	chown mysql:mysql "$DB_RUNTIME_DIR"

	log_success "MariaDB runtime directory prepared."
}

initialize_database() {
	if test -d "$DB_DATA_DIR/mysql"; then
		log_info "Existing MariaDB system tables detected. Skipping initialization."
		return 0
	fi

	log_info "Initializing MariaDB data directory..."

	if ! mariadb-install-db --user=mysql --datadir="$DB_DATA_DIR"; then
		log_error "MariaDB initialization failed. Inspect $DB_DATA_DIR manually."
		exit 1
	fi

	log_success "MariaDB data directory initialized."
}

configure_database() {
	log_info "Configuring application database and user..."

	if ! mariadbd --user=mysql --datadir="$DB_DATA_DIR" \
		--bootstrap --skip-networking << EOF
FLUSH PRIVILEGES;

CREATE DATABASE IF NOT EXISTS \`$DB_NAME\`;

CREATE USER IF NOT EXISTS '$DB_USER'@'%'
IDENTIFIED BY '$DB_PASSWORD';

ALTER USER '$DB_USER'@'%'
IDENTIFIED BY '$DB_PASSWORD';

GRANT ALL PRIVILEGES ON \`$DB_NAME\`.*
TO '$DB_USER'@'%';

ALTER USER 'root'@'localhost'
IDENTIFIED VIA mysql_native_password USING PASSWORD('$DB_ROOT_PASSWORD');
EOF
	then
		log_error "Failed to configure the database in bootstrap mode."
		exit 1
	fi

	log_success "Application database and user configured."
}

main() {
	log_info "Starting MariaDB setup..."

	validate_env
	validate_passwords

	prepare_runtime
	initialize_database
	configure_database

	log_success "MariaDB setup completed."

	exec "$@"
}

main "$@"
