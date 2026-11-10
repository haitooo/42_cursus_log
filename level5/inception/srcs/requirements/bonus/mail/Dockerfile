FROM debian:bookworm

RUN echo "postfix postfix/main_mailer_type select No configuration" \
		| debconf-set-selections && \
	apt-get update -e any && \
	DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
		postfix \
		libsasl2-modules \
		ca-certificates && \
	rm -rf /var/lib/apt/lists/*

COPY conf/main.cf /etc/postfix/main.cf

RUN postconf -F '*/*/chroot = n'

COPY tools/mail_setup.sh /usr/local/bin/

RUN chmod +x /usr/local/bin/mail_setup.sh

EXPOSE 25

ENTRYPOINT ["/usr/local/bin/mail_setup.sh"]
CMD ["postfix", "start-fg"]
