.PHONY: ambient

CCFLAGS := -g3 -O0 -W -Wall -std=gnu11 -pipe
LDFLAGS := -lpthread -lcap-ng

EXT_CFLAG_NO_WARINGS = -Wno-int-to-pointer-cast -Wno-unused-parameter -Wno-pointer-to-int-cast -Wno-unused-variable

ambient: main.c
	@echo "test Semi-Privileged Environments(in the sense of process trees or namespaces) Using ambient and inheritable sets ..."
	gcc $(CCFLAGS) $(EXT_CFLAG_NO_WARINGS) $^ -o $@ $(LDFLAGS)
