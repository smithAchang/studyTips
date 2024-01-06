.PHONY: app dynaArray
app: dynaArray

CCFLAGS := -g3 -O0 -W -Wall -std=gnu11 -pipe
LDFLAGS := -lpthread
EXT_NO_WARNINGS = -Wno-int-to-pointer-cast -Wno-unused-parameter -Wno-pointer-to-int-cast -Wno-unused-variable

vpath %.h .
vpath %.c .

dynaArray: dynaArray.c
	@echo "build dynamic stack array usage program ..."
	gcc $(CCFLAGS) $(EXT_NO_WARNINGS) $^ -o $@ $(LDFLAGS)
