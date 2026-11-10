#!/bin/sh
set -e

WP_DIR="/var/www/html"
CACHE_DROPIN="$WP_DIR/wp-content/object-cache.php"
MAIL_PLUGIN="$WP_DIR/wp-content/mu-plugins/inception-smtp.php"

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
	case "${REDIS_HOST}" in
		*[!a-zA-Z0-9.-]*)
			log_error "REDIS_HOST contains invalid characters."
			exit 1
			;;
	esac

	case "${MAIL_HOST}" in
		*[!a-zA-Z0-9.-]*)
			log_error "MAIL_HOST contains invalid characters."
			exit 1
			;;
	esac
}

check_redis_connection() {
	REDIS_HOST="$REDIS_HOST" php -r '
		$socket = @fsockopen(getenv("REDIS_HOST"), 6379, $errno, $errstr, 3);
		if ($socket === false) {
			fwrite(STDERR, "$errstr\n");
			exit(1);
		}
		fclose($socket);
	'
}

wait_for_redis() {
	log_info "Waiting for Redis at $REDIS_HOST:6379..."

	attempt=0
	until redis_error=$(check_redis_connection 2>&1)
	do
		attempt=$((attempt + 1))

		if test "$attempt" -ge 30; then
			printf '\n'
			log_error "Redis did not become ready after 30 attempts: $redis_error"
			exit 1
		fi

		printf "\r${YELLOW}[WAIT]${RESET} Waiting for Redis... (%d/30)" "$attempt"
		sleep 1
	done

	if test "$attempt" -gt 0; then
		printf '\n'
	fi

	log_success "Connected to Redis."
}

prepare_redis_cache() {
	if test -z "${REDIS_HOST}"; then
		if test -f "$CACHE_DROPIN"; then
			log_info "REDIS_HOST is not set. Removing the Redis object cache drop-in..."
			rm -f "$CACHE_DROPIN"
			log_success "Redis object cache drop-in removed."
		fi
		return 0
	fi

	wait_for_redis
}

configure_redis() {
	if test -z "${REDIS_HOST}"; then
		if wp_cli plugin is-active redis-cache; then
			wp_cli plugin deactivate redis-cache >/dev/null
			log_success "Redis object cache plugin deactivated."
		fi
		return 0
	fi

	log_info "Configuring the Redis object cache at $REDIS_HOST:6379..."

	if ! wp_cli plugin is-installed redis-cache; then
		wp_cli plugin install redis-cache >/dev/null
	fi

	wp_cli plugin activate redis-cache >/dev/null
	wp_cli config set WP_REDIS_HOST "$REDIS_HOST" --quiet
	wp_cli config set WP_REDIS_PORT 6379 --raw --quiet

	if test ! -f "$CACHE_DROPIN"; then
		wp_cli redis enable >/dev/null
	fi

	log_success "Redis object cache enabled."
}

configure_mail() {
	if test -z "${MAIL_HOST}"; then
		if test -f "$MAIL_PLUGIN"; then
			rm -f "$MAIL_PLUGIN"
			log_success "SMTP plugin removed."
		fi
		wp_cli option update users_can_register 0 --quiet
		return 0
	fi

	log_info "Configuring WordPress to send e-mails through $MAIL_HOST:25..."

	mkdir -p "$(dirname "$MAIL_PLUGIN")"
	cat > "$MAIL_PLUGIN" << EOF
<?php
/*
 * Plugin Name: Inception SMTP
 * Description: Send WordPress e-mails through the mail container. Written by wp_bonus_setup.sh.
 */
add_action('phpmailer_init', function (\$phpmailer) {
	\$phpmailer->isSMTP();
	\$phpmailer->Host = '$MAIL_HOST';
	\$phpmailer->Port = 25;
	\$phpmailer->SMTPAuth = false;
	\$phpmailer->SMTPAutoTLS = false;
});
EOF

	wp_cli option update users_can_register 1 --quiet
	wp_cli option update default_role subscriber --quiet

	log_success "E-mail sending and self-registration enabled."
}

main() {
	validate_env

	case "$1" in
		before-install)
			prepare_redis_cache
			;;
		after-install)
			configure_redis
			configure_mail
			;;
		*)
			log_error "Usage: wp_bonus_setup.sh before-install|after-install"
			exit 1
			;;
	esac
}

main "$@"
