PREFIX = /usr/local

tao: main.c
	gcc tao.c -lmagic -g -o tao

XDG_CONFIG_HOME ?= ${HOME}/.config

.PHONY: config
config: example.conf
	mkdir -p "$(XDG_CONFIG_HOME)"
	cp example.conf "$(XDG_CONFIG_HOME)/tao.conf"

.PHONY: install
install: tao
	mkdir -p ${DESTDIR}${PREFIX}/bin/
	cp tao ${DESTDIR}${PREFIX}/bin/tao

.PHONY: uninstall
uninstall:
	rm ${DESTDIR}${PREFIX}/bin/tao
