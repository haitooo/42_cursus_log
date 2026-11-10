#!/bin/sh
set -e

MAIL_PASS_PATH="/run/secrets/mail_password"
SASL_PASSWD_PATH="/etc/postfix/sasl_passwd"

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

	if test -z "${MAIL_RELAY_HOST}"; then
		log_error "MAIL_RELAY_HOST is required."
		exit 1
	fi

	if test -z "${MAIL_RELAY_PORT}"; then
		log_error "MAIL_RELAY_PORT is required."
		exit 1
	fi

	if test -z "${MAIL_RELAY_USER}"; then
		log_error "MAIL_RELAY_USER is required."
		exit 1
	fi

	case "$MAIL_RELAY_HOST" in
		*[!a-zA-Z0-9.-]*)
			log_error "MAIL_RELAY_HOST contains invalid characters."
			exit 1
			;;
	esac

	case "$MAIL_RELAY_PORT" in
		*[!0-9]*)
			log_error "MAIL_RELAY_PORT must be a number."
			exit 1
			;;
	esac

	case "$MAIL_RELAY_USER" in
		*[!a-zA-Z0-9._%+@-]*)
			log_error "MAIL_RELAY_USER contains invalid characters."
			exit 1
			;;
	esac

	log_success "Mail relay configuration validated."
}

validate_password() {
	log_info "Validating mail relay password secret..."

	if test ! -f "$MAIL_PASS_PATH" || test ! -r "$MAIL_PASS_PATH"; then
		log_error "Mail relay password secret is missing or unreadable."
		exit 1
	fi

	MAIL_PASSWORD=$(tr -d ' \n' < "$MAIL_PASS_PATH")

	if test -z "$MAIL_PASSWORD"; then
		log_error "Mail relay password secret is empty."
		exit 1
	fi

	case "$MAIL_PASSWORD" in
		*:*)
			log_error "Mail relay password must not contain ':'."
			exit 1
			;;
	esac

	log_success "Mail relay password validated."
}

configure_relay() {
	log_info "Configuring relay to $MAIL_RELAY_HOST:$MAIL_RELAY_PORT..."

	relay="[$MAIL_RELAY_HOST]:$MAIL_RELAY_PORT"

	(
		umask 077
		printf '%s %s:%s\n' "$relay" "$MAIL_RELAY_USER" "$MAIL_PASSWORD" \
			> "$SASL_PASSWD_PATH"
	)

	postconf -e "relayhost = $relay"

	log_success "Relay configured for $MAIL_RELAY_USER."
}

check_config() {
	log_info "Checking Postfix configuration..."

	if ! postfix check; then
		log_error "Postfix configuration check failed."
		exit 1
	fi

	log_success "Postfix configuration is valid."
}

main() {
	log_info "Starting mail setup..."

	validate_env
	validate_password

	configure_relay
	check_config

	log_success "Mail setup completed."

	exec "$@"
}

main "$@"
