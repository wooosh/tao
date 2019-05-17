tao: main.c
	gcc main.c -lmagic -g -o tao

XDG_CONFIG_HOME ?= ${HOME}/.config

.PHONY: config
config: example.conf
	mkdir -p "$(XDG_CONFIG_HOME)"
	cp example.conf "$(XDG_CONFIG_HOME)/tao.conf"

.PHONY: install
install: tao
	cp tao /usr/local/bin/tao

.PHONY: uninstall
uninstall:
	rm /usr/local/bin/tao
