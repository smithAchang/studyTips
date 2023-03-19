.PHONY: app rebind
app: rebind

vpath %.h .
vpath %.c .

rebind: rawbind.c header.h
	@echo "build raw udp client program ..."
	gcc -g3 -O0 -W -Wall -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-variable $^ -o $@ 
