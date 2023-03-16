.PHONY: app clientu clientr server
app:  clientu clientr


clientu: rawudp.c 
	@echo "build raw udp client program ..."
	gcc -g3 -O0 -W -Wall -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-variable $^ -o $@ 

clientr: rawall.c 
	@echo "build raw all socket client program ..."
	gcc -g3 -O0 -W -Wall -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-variable $^ -o $@ 
