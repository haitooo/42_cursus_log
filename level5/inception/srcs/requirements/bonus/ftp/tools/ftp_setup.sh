#!/bin/sh
set -e

FTP_PASS_PATH="/run/secrets/ftp_password"
FTP_ROOT="/var/www/html"
FTP_RUNTIME_DIR="/var/run/vsftpd/empty"
FILE_OWNER="www-data"

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

	if test -z "${FTP_USER}"; then
		log_error "FTP_USER is required."
		exit 1
	fi

	case "$FTP_USER" in
		*[!a-z0-9_-]*)
			log_error "FTP_USER may only contain lowercase letters, digits, '_' and '-'."
			exit 1
			;;
	esac

	if test "${#FTP_USER}" -gt 32; then
		log_error "FTP_USER must be at most 32 characters."
		exit 1
	fi

	if getent passwd "$FTP_USER" >/dev/null \
		&& test "$(id -u "$FTP_USER")" != "$(id -u "$FILE_OWNER")"; then
		log_error "FTP_USER must not be an existing system user."
		exit 1
	fi

	if test "$FTP_USER" = "$FILE_OWNER"; then
		log_error "FTP_USER must not be $FILE_OWNER."
		exit 1
	fi

	log_success "FTP configuration validated."
}

validate_password() {
	log_info "Validating FTP password secret..."

	if test ! -f "$FTP_PASS_PATH" || test ! -r "$FTP_PASS_PATH"; then
		log_error "FTP password secret is missing or unreadable."
		exit 1
	fi

	FTP_PASSWORD=$(cat "$FTP_PASS_PATH")

	if test "${#FTP_PASSWORD}" -lt 8; then
		log_error "FTP password must be at least 8 characters."
		exit 1
	fi

	if test "${#FTP_PASSWORD}" -gt 128; then
		log_error "FTP password must not exceed 128 characters."
		exit 1
	fi

	case "$FTP_PASSWORD" in
		*[!a-zA-Z0-9_!@#%+=?-]*)
			log_error "FTP password contains unsupported characters."
			exit 1
			;;
	esac

	log_success "FTP password validated."
}

prepare_runtime() {
	log_info "Preparing vsftpd runtime directory..."

	mkdir -p "$FTP_RUNTIME_DIR"

	log_success "vsftpd runtime directory prepared."
}

create_ftp_user() {
	if getent passwd "$FTP_USER" >/dev/null; then
		log_info "User $FTP_USER already exists. Skipping creation."
		return 0
	fi

	log_info "Creating FTP user $FTP_USER..."

	useradd \
		--non-unique \
		--uid "$(id -u "$FILE_OWNER")" \
		--gid "$(id -g "$FILE_OWNER")" \
		--home-dir "$FTP_ROOT" \
		--no-create-home \
		--shell /bin/sh \
		"$FTP_USER"

	log_success "FTP user $FTP_USER created."
}

set_password() {
	log_info "Setting password for $FTP_USER..."

	printf '%s:%s\n' "$FTP_USER" "$FTP_PASSWORD" | chpasswd

	log_success "Password set."
}

main() {
	log_info "Starting FTP setup..."

	validate_env
	validate_password

	prepare_runtime
	create_ftp_user
	set_password

	log_success "FTP setup completed."

	exec "$@"
}

main "$@"
