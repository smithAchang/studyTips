.PHONY: app rebind
app: rebind

CCFLAGS := -g3 -O0 -W -Wall -std=gnu11 -pipe
LDFLAGS := -lpthread

EXT_CFLAG_NO_WARINGS =  -Wno-int-to-pointer-cast -Wno-unused-parameter -Wno-pointer-to-int-cast -Wno-unused-variable
EXT_CFLAG_NO_WARINGS += -Wno-unused-function

vpath %.h .:../common
vpath %.c .:../common

rebind: rawbind.c header.h
	@echo "build raw udp client program ..."
	gcc $(CCFLAGS) $(EXT_CFLAG_NO_WARINGS) $^ -o $@ $(LDFLAGS)
