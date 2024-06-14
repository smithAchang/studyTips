.PHONY: signalProc

CCFLAGS := -g3 -O0 -W -Wall -std=gnu11 -pipe -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-variable -Wno-unused-but-set-variable

signalProc: main.c
	@echo "test default signal process program ..."
	gcc $(CCFLAGS) $^ $(LDFLAGS)
