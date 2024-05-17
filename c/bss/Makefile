.PHONY: globalMemoryUsage

CCFLAGS := -g3 -O0 -W -Wall -std=gnu11 -pipe -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-variable -Wno-unused-but-set-variable

globalMemoryUsage: main.c
	@echo "test globalMemoryUsage program ..."
	gcc $(CCFLAGS) $^ $(LDFLAGS)
