FROM debian:bookworm

RUN apt-get update -e any && \
	apt-get install -y --no-install-recommends redis-server && \
	rm -rf /var/lib/apt/lists/*

COPY conf/redis.conf /etc/redis/inception.conf

USER redis

CMD ["redis-server", "/etc/redis/inception.conf"]
