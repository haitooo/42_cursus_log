FROM debian:bookworm

RUN apt-get update -e any && \
	apt-get install -y --no-install-recommends busybox && \
	rm -rf /var/lib/apt/lists/*

COPY site/ /var/www/website/

EXPOSE 8081

CMD ["busybox", "httpd", "-f", "-v", "-p", "8081", "-u", "www-data", "-h", "/var/www/website"]
