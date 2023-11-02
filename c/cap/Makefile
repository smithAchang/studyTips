.PHONY: checkraw

CCFLAGS := -g3 -O0 -W -Wall -std=gnu11 -pipe -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-variable -Wno-unused-but-set-variable
LDFLAGS := -lpthread

checkraw: main.c
	@echo "test raw socket support using sock API ..."
	gcc $(CCFLAGS) $^ -o $@ $(LDFLAGS)
