.PHONY: app voidptr
app: voidptr

CCFLAGS := -g3 -O0 -W -Wall -std=gnu11 -pipe
LDFLAGS := -lpthread

EXT_CFLAG_NO_WARINGS =  -Wno-int-to-pointer-cast -Wno-unused-parameter -Wno-pointer-to-int-cast -Wno-unused-variable
EXT_CFLAG_NO_WARINGS += -Wno-unused-function

vpath %.h .
vpath %.c .

voidptr: voidptr.c
	@echo "build void* ptr usage test program ..."
	gcc $(CCFLAGS) $(EXT_CFLAG_NO_WARINGS) $^ -o $@ $(LDFLAGS)
