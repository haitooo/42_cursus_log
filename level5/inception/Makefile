LOGIN			:= haito
DATA_DIR		:= /home/$(LOGIN)/data
VOLUME_DIRS		:= $(DATA_DIR)/mariadb $(DATA_DIR)/wordpress

COMPOSE_FILE	:= srcs/docker-compose.yml
BONUS_FILE		:= srcs/docker-compose.bonus.yml
ENV_FILE		:= srcs/.env
SECRET_FILES	:= secrets/db_password.txt \
				   secrets/db_root_password.txt \
				   secrets/credentials.txt
BONUS_SECRETS	:= secrets/ftp_password.txt \
				   secrets/mail_password.txt

COMPOSE			:= docker compose -f $(COMPOSE_FILE)
# Both files: used to start the bonus services, and to stop or remove every
# container of the project whether the bonus services were started or not.
COMPOSE_ALL		:= docker compose -f $(COMPOSE_FILE) -f $(BONUS_FILE)

CLEANER_IMAGE	:= debian:bookworm

all: up

up: check_files $(VOLUME_DIRS)
	$(COMPOSE) up -d --build

bonus: check_files check_bonus_files $(VOLUME_DIRS)
	$(COMPOSE_ALL) up -d --build

build: check_files
	$(COMPOSE) build

down:
	$(COMPOSE_ALL) down

stop:
	$(COMPOSE_ALL) stop

start:
	$(COMPOSE_ALL) start

ps:
	$(COMPOSE_ALL) ps

logs:
	$(COMPOSE_ALL) logs -f

mariadb:
	$(COMPOSE) exec mariadb bash

nginx:
	$(COMPOSE) exec nginx bash

wordpress:
	$(COMPOSE) exec wordpress bash

check_files:
	@for f in $(ENV_FILE) $(SECRET_FILES); do \
		if test ! -f "$$f"; then \
			echo "Error: $$f is missing. See DEV_DOC.md." >&2; \
			exit 1; \
		fi; \
	done

check_bonus_files:
	@for f in $(BONUS_SECRETS); do \
		if test ! -f "$$f"; then \
			echo "Error: $$f is missing. See DEV_DOC.md." >&2; \
			exit 1; \
		fi; \
	done

$(VOLUME_DIRS):
	mkdir -p $@

clean:
	$(COMPOSE_ALL) down --volumes --rmi all --remove-orphans

fclean: clean
	@if test -d "$(DATA_DIR)"; then \
		docker run --rm -v "$(DATA_DIR):/data" $(CLEANER_IMAGE) \
			rm -rf /data/mariadb /data/wordpress; \
	fi

re: fclean all

.PHONY: all up bonus build down stop start ps logs mariadb nginx wordpress \
	check_files check_bonus_files clean fclean re
