FROM debian:bookworm

RUN apt-get update -e any && \
	apt-get install -y --no-install-recommends \
		nginx \
		openssl && \
	rm -rf /var/lib/apt/lists/* /var/www/html/*

COPY conf/nginx.conf /etc/nginx/nginx.conf
COPY tools/nginx_setup.sh /usr/local/bin/

RUN chmod +x /usr/local/bin/nginx_setup.sh

ENTRYPOINT ["/usr/local/bin/nginx_setup.sh"]
CMD ["/usr/sbin/nginx", "-g", "daemon off;"]
