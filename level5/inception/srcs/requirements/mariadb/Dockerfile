FROM debian:bookworm

RUN apt-get update -e any && \
	apt-get install -y --no-install-recommends mariadb-server && \
	rm -rf /var/lib/apt/lists/*

COPY conf/60-inception.cnf /etc/mysql/mariadb.conf.d/
COPY tools/db_setup.sh /usr/local/bin/

RUN chmod +x /usr/local/bin/db_setup.sh

ENTRYPOINT ["/usr/local/bin/db_setup.sh"]
CMD ["mariadbd", "--user=mysql", "--console"]
