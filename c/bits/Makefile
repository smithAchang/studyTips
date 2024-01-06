.PHONY: app bitincr
app: bitincr

CCCFLAGS := -g3 -O0 -W -Wall -std=gnu11 -pipe
LDFLAGS := -lpthread
EXT_CFLAG_NO_WARINGS = -Wno-int-to-pointer-cast -Wno-unused-parameter -Wno-pointer-to-int-cast -Wno-unused-variable


vpath %.h .
vpath %.c .

bitincr: bitincr.c
	@echo "build bits field increased beyond the threshold test program ..."
	gcc $(CCFLAGS) $(EXT_CFLAG_NO_WARINGS) $^ -o $@ $(LDFLAGS)
