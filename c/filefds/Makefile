.PHONY: filefds

CCFLAGS := -g3 -O0 -W -Wall -std=gnu11 -pipe -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter
LDFLAGS := -lpthread

filefds: main.c
	@echo "test opened file fds of process ..."
	gcc $(CCFLAGS) $^ $(LDFLAGS)
