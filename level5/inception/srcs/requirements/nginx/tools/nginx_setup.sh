#!/bin/sh
set -e

TLS_DIR="/etc/nginx/ssl"
CERT_PATH="$TLS_DIR/inception.crt"
KEY_PATH="$TLS_DIR/inception.key"

BLUE='\033[0;34m'
GREEN='\033[0;32m'
RED='\033[0;31m'
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

	if test -z "${DOMAIN_NAME}"; then
		log_error "DOMAIN_NAME is required."
		exit 1
	fi

	case "$DOMAIN_NAME" in
		*[!a-zA-Z0-9.-]*)
			log_error "DOMAIN_NAME contains invalid characters."
			exit 1
			;;
	esac

	log_success "NGINX configuration validated."
}

prepare_certificate() {
	if test -s "$CERT_PATH" && test -s "$KEY_PATH"; then
		log_info "Existing TLS certificate and key found. Skipping generation."
		return 0
	fi

	log_info "Creating a self-signed certificate for $DOMAIN_NAME..."

	if ! (
		umask 077
		mkdir -p "$TLS_DIR"
		openssl req -x509 -newkey rsa:2048 -noenc -days 365 \
			-subj "/CN=$DOMAIN_NAME" \
			-addext "subjectAltName=DNS:$DOMAIN_NAME" \
			-keyout "$KEY_PATH.pending" \
			-out "$CERT_PATH.pending"
	); then
		log_error "Failed to create the TLS certificate and key."
		exit 1
	fi

	mv "$KEY_PATH.pending" "$KEY_PATH"
	mv "$CERT_PATH.pending" "$CERT_PATH"

	log_success "Self-signed certificate created."
}

check_config() {
	log_info "Checking NGINX configuration..."

	if ! nginx -t; then
		log_error "NGINX configuration check failed."
		exit 1
	fi

	log_success "NGINX configuration is valid."
}

main() {
	log_info "Starting NGINX setup..."

	validate_env
	prepare_certificate
	check_config

	log_success "NGINX setup completed."

	exec "$@"
}

main "$@"
