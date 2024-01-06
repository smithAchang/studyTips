.PHONY: app clientu clientr server flowlabel
app:  clientu clientr server flowlabel pkginfo

CCFLAGS := -g3 -O0 -W -Wall -std=gnu11 -pipe
LDFLAGS := -lpthread

EXT_CFLAG_NO_WARINGS =  -Wno-int-to-pointer-cast -Wno-unused-parameter -Wno-pointer-to-int-cast -Wno-unused-variable
EXT_CFLAG_NO_WARINGS += -Wno-unused-function

vpath %.h .:../common
vpath %.c .:../common



clientu: rawudp.c header.h
	@echo "build raw udp client program ..."
	gcc $(CCFLAGS) $(EXT_CFLAG_NO_WARINGS) -I../common $^ -o $@ $(LDFLAGS)

clientr: rawall.c header.h
	@echo "build raw all socket client program ..."
	gcc $(CCFLAGS) $(EXT_CFLAG_NO_WARINGS) -I../common $^ -o $@ $(LDFLAGS)

server: server.c header.h
	@echo "build raw socket server program ..."
	gcc $(CCFLAGS) $(EXT_CFLAG_NO_WARINGS) -I../common $^ -o $@ $(LDFLAGS)

flowlabel: flowlabel.c header.h
	@echo "build IPv6 flow label test program ..."
	gcc $(CCFLAGS) $(EXT_CFLAG_NO_WARINGS) -I../common $^ -o $@ $(LDFLAGS)
