FROM debian:bookworm

RUN apt-get update -e any && \
	apt-get install -y --no-install-recommends \
		 php8.2-fpm \
		php8.2-cli \
		php8.2-mysql \
		curl \
		ca-certificates && \
	rm -rf /var/lib/apt/lists/*

ARG WP_CLI_VER=2.12.0

RUN curl -fsSL "https://github.com/wp-cli/wp-cli/releases/download/v${WP_CLI_VER}/wp-cli-${WP_CLI_VER}.phar" \
	-o /usr/local/bin/wp && \
	chmod +x /usr/local/bin/wp && \
	mkdir -p /run/php

COPY conf/www.conf /etc/php/8.2/fpm/pool.d/www.conf
COPY tools/wp_setup.sh /usr/local/bin/

RUN chmod +x /usr/local/bin/wp_setup.sh

WORKDIR /var/www/html

ENTRYPOINT [ "/bin/sh", "-c", "/usr/local/bin/wp_setup.sh && exec /usr/sbin/php-fpm8.2 -F" ]
