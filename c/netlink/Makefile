.PHONY: app getroute
app: getroute

CCFLAGS := -g3 -O0 -W -Wall -std=gnu11 -pipe
LDFLAGS := -lpthread
EXT_NO_WARNINGS = -Wno-int-to-pointer-cast -Wno-unused-parameter -Wno-pointer-to-int-cast -Wno-unused-variable

vpath %.h .
vpath %.c .

getroute: getroute.c
	@echo "create Preprocess only; do not compile, assemble or link result..."
	gcc $(CCFLAGS) $(EXT_NO_WARNINGS) -E $^ -o $^.preprocess.txt
	@echo "build getroute program ..."
	gcc $(CCFLAGS) $(EXT_NO_WARNINGS) $^ -o $@ $(LDFLAGS)
