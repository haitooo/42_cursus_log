#!/bin/sh
set -e

DB_PASS_PATH="/run/secrets/db_password"
DB_ROOT_PASS_PATH="/run/secrets/db_root_password"
DB_DATA_DIR="/var/lib/mysql"
DB_RUNTIME_DIR="/run/mysqld"
ROOT_CLIENT_CONFIG="/run/mariadb-root-client.cnf"

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
RESET='\033[0m'

server_pid=""

log_info() {
	printf "${BLUE}[INFO]${RESET} %s\n" "$1"
}

log_success() {
	printf "${GREEN}[ OK ]${RESET} %s\n" "$1"
}

log_warn() {
	printf "${YELLOW}[WARN]${RESET} %s\n" "$1"
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

create_root_client_config() {
	(
		umask 077
		printf '[client]\nuser=root\npassword="%s"\nprotocol=socket\n' \
			"$DB_ROOT_PASSWORD" > "$ROOT_CLIENT_CONFIG"
	)
}

mariadb_root() {
	mariadb --defaults-extra-file="$ROOT_CLIENT_CONFIG" "$@"
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

start_temporary_server() {
	log_info "Starting temporary MariaDB server..."

	mariadbd --user=mysql --datadir="$DB_DATA_DIR" --skip-networking &

	server_pid=$!

	attempt=0
	until root_error=$(mariadb_root -e "SELECT 1" 2>&1)
	do
		attempt=$((attempt + 1))

		# Access denied will not fix itself by waiting: the root password in
		# the secret differs from the one stored in the existing database.
		case "$root_error" in
			*"ERROR 1045"*)
				printf '\n'
				log_error "db_root_password does not match the root password stored in $DB_DATA_DIR."
				log_error "Changing db_root_password after initialization is not supported. See README."
				exit 1
				;;
		esac

		if ! kill -0 "$server_pid" 2>/dev/null; then
			log_error "Temporary MariaDB server exited unexpectedly."
			exit 1
		fi

		if test "$attempt" -ge 30; then
			printf '\n'
			log_error "MariaDB did not become ready after 30 attempts: $root_error"
			exit 1
		fi

		printf "\r${YELLOW}[WAIT]${RESET} Waiting for MariaDB... (%d/30)" "$attempt"
		sleep 1
	done

	if test "$attempt" -gt 0; then
		printf '\n'
	fi

	log_success "Temporary MariaDB server is ready."
}

configure_database() {
	log_info "Configuring application database and user..."

	mariadb_root << EOF
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

	log_success "Application database and user configured."
}

stop_temporary_server() {
	log_info "Stopping temporary MariaDB server..."

	mariadb-admin --defaults-extra-file="$ROOT_CLIENT_CONFIG" shutdown

	wait "$server_pid"
	server_pid=""

	log_success "Temporary MariaDB server stopped."
}

cleanup() {
	if test -n "$server_pid"; then
		log_warn "Cleaning up temporary MariaDB server..."

		if kill -0 "$server_pid" 2>/dev/null; then
			mariadb-admin --defaults-extra-file="$ROOT_CLIENT_CONFIG" shutdown \
				>/dev/null 2>&1 || kill "$server_pid" 2>/dev/null || :
		fi

		wait "$server_pid" 2>/dev/null || :
		server_pid=""
	fi

	rm -f "$ROOT_CLIENT_CONFIG"
}

main() {
	log_info "Starting MariaDB setup..."

	validate_env
	validate_passwords

	prepare_runtime

	trap 'exit 143' TERM
	trap 'exit 130' INT
	trap 'cleanup' EXIT

	create_root_client_config
	initialize_database
	start_temporary_server
	configure_database
	stop_temporary_server
	cleanup

	trap - EXIT TERM INT

	log_success "MariaDB setup completed."
}

main
