FROM debian:bookworm

RUN apt-get update -e any && \
	apt-get install -y --no-install-recommends \
		adminer \
		php8.2-cli \
		php8.2-mysql && \
	rm -rf /var/lib/apt/lists/*

USER www-data

EXPOSE 8080

CMD ["php", "-S", "0.0.0.0:8080", "/usr/share/adminer/adminer.php"]
