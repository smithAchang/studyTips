.PHONY: test

CCFLAGS := -g3 -O0 -W -Wall -std=gnu11 -pipe
LDFLAGS := -lpthread

EXT_CFLAG_NO_WARINGS =  -Wno-int-to-pointer-cast -Wno-unused-parameter -Wno-pointer-to-int-cast -Wno-unused-variable
EXT_CFLAG_NO_WARINGS += -Wno-unused-function

test: main.c 
	@echo "feature test program ..."
	gcc $(CCFLAGS) $(EXT_CFLAG_NO_WARINGS) $^ $(LDFLAGS)
