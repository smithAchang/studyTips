.PHONY: app clientu clientr server flowlabel
app:  clientu clientr server flowlabel pkginfo

EXT_CFLAG_NO_WARINGS =  -Wno-int-to-pointer-cast -Wno-unused-parameter -Wno-pointer-to-int-cast -Wno-unused-variable
EXT_CFLAG_NO_WARINGS += -Wno-unused-function

vpath %.h .:../common
vpath %.c .:../common



clientu: rawudp.c header.h
	@echo "build raw udp client program ..."
	gcc -g3 -O0 -W -Wall $(EXT_CFLAG_NO_WARINGS) -I../common $^ -o $@ 

clientr: rawall.c header.h
	@echo "build raw all socket client program ..."
	gcc -g3 -O0 -W -Wall $(EXT_CFLAG_NO_WARINGS) -I../common $^ -o $@

server: server.c header.h
	@echo "build raw socket server program ..."
	gcc -g3 -O0 -W -Wall $(EXT_CFLAG_NO_WARINGS) -I../common $^ -o $@ 

flowlabel: flowlabel.c header.h
	@echo "build IPv6 flow label test program ..."
	gcc -g3 -O0 -W -Wall $(EXT_CFLAG_NO_WARINGS) -I../common $^ -o $@ 
