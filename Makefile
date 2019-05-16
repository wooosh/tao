tao: main.c
	gcc main.c -lmagic -g -o tao

.PHONY: config
config: example.conf
	mkdir -p ~/.config
	cp example.conf ~/.config/tao.conf

.PHONY: install
install: tao
	cp tao /usr/local/bin/tao

.PHONY: uninstall
uninstall:
	rm /usr/local/bin/tao
