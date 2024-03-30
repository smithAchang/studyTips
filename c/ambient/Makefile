.PHONY: ambient

CCFLAGS := -g3 -O0 -W -Wall -std=gnu11 -pipe -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-variable -Wno-unused-but-set-variable
LDFLAGS := -lpthread -lcap-ng

ambient: main.c
	@echo "test Semi-Privileged Environments(in the sense of process trees or namespaces) Using ambient and inheritable sets ..."
	gcc $(CCFLAGS) $^ -o $@ $(LDFLAGS)
