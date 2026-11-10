LOGIN			:= haito
DATA_DIR		:= /home/$(LOGIN)/data
VOLUME_DIRS		:= $(DATA_DIR)/mariadb $(DATA_DIR)/wordpress

COMPOSE_FILE	:= srcs/docker-compose.yml
ENV_FILE		:= srcs/.env
SECRET_FILES	:= secrets/db_password.txt \
				   secrets/db_root_password.txt \
				   secrets/credentials.txt

COMPOSE			:= docker compose -f $(COMPOSE_FILE)

CLEANER_IMAGE	:= debian:bookworm

all: up

up: check_files $(VOLUME_DIRS)
	$(COMPOSE) up -d --build

build: check_files
	$(COMPOSE) build

down:
	$(COMPOSE) down

stop:
	$(COMPOSE) stop

start:
	$(COMPOSE) start

ps:
	$(COMPOSE) ps

logs:
	$(COMPOSE) logs -f

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

$(VOLUME_DIRS):
	mkdir -p $@

clean:
	$(COMPOSE) down --volumes --rmi all --remove-orphans

fclean: clean
	@if test -d "$(DATA_DIR)"; then \
		docker run --rm -v "$(DATA_DIR):/data" $(CLEANER_IMAGE) \
			rm -rf /data/mariadb /data/wordpress; \
	fi

re: fclean all

.PHONY: all up build down stop start ps logs check_files clean fclean re
