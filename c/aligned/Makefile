.PHONY: main

CCFLAGS := -g3 -O0 -W -Wall -std=gnu11 -pipe -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter
LDFLAGS := 

main: main.c
	@echo "Access struct member in default and access struct member with alignment ..."
	gcc $(CCFLAGS) $^ $(LDFLAGS)
