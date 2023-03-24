.PHONY: app clientu clientr server flowlabel
app:  clientu clientr server flowlabel pkginfo

vpath %.h .
vpath %.c .

clientu: rawudp.c header.h
	@echo "build raw udp client program ..."
	gcc -g3 -O0 -W -Wall -Wno-int-to-pointer-cast -Wno-unused-parameter -Wno-pointer-to-int-cast -Wno-unused-variable $^ -o $@ 

clientr: rawall.c header.h
	@echo "build raw all socket client program ..."
	gcc -g3 -O0 -W -Wall -Wno-int-to-pointer-cast -Wno-unused-parameter -Wno-pointer-to-int-cast -Wno-unused-variable $^ -o $@

server: server.c header.h
	@echo "build raw socket server program ..."
	gcc -g3 -O0 -W -Wall -Wno-int-to-pointer-cast -Wno-unused-parameter -Wno-pointer-to-int-cast -Wno-unused-variable $^ -o $@ 

flowlabel: flowlabel.c header.h
	@echo "build IPv6 flow label test program ..."
	gcc -g3 -O0 -W -Wall -Wno-int-to-pointer-cast -Wno-unused-parameter -Wno-pointer-to-int-cast -Wno-unused-variable $^ -o $@ 

pkginfo: pkginfo.c header.h
	@echo "build IPv6 pkginfo test program ..."
	gcc -g3 -O0 -W -Wall -Wno-int-to-pointer-cast -Wno-unused-parameter -Wno-pointer-to-int-cast -Wno-unused-variable $^ -o $@ 
