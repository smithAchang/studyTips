.PHONY: app clientu clientr server
app:  clientu clientr server

vpath %.h .
vpath %.c .

clientu: rawudp.c header.h
	@echo "build raw udp client program ..."
	gcc -g3 -O0 -W -Wall -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-variable $^ -o $@ 

clientr: rawall.c header.h
	@echo "build raw all socket client program ..."
	gcc -g3 -O0 -W -Wall -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-variable $^ -o $@

server: server.c header.h
	@echo "build raw socket server program ..."
	gcc -g3 -O0 -W -Wall -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-variable $^ -o $@ 
