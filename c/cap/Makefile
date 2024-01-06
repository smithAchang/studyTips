.PHONY: checkraw

CCFLAGS := -g3 -O0 -W -Wall -std=gnu11 -pipe
LDFLAGS := -lpthread

EXT_CFLAG_NO_WARINGS = -Wno-int-to-pointer-cast -Wno-unused-parameter -Wno-pointer-to-int-cast -Wno-unused-variable

checkraw: main.c
	@echo "test raw socket support using sock API ..."
	gcc $(CCFLAGS) $(EXT_CFLAG_NO_WARINGS) $^ -o $@ $(LDFLAGS)
