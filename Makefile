PREFIX = /usr/local
XDG_CONFIG_HOME ?= ${HOME}/.config

all: tao.c
	gcc tao.c -lmagic -g -o tao

config: example.conf
	mkdir -p "${XDG_CONFIG_HOME}"
	cp example.conf "${XDG_CONFIG_HOME}/tao.conf"

install: tao
	mkdir -p ${DESTDIR}${PREFIX}/bin/
	cp tao ${DESTDIR}${PREFIX}/bin/tao

uninstall:
	rm ${DESTDIR}${PREFIX}/bin/tao

.PHONY: config install uninstall