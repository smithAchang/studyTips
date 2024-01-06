.PHONY: app main
app:  main

CCFLAGS := -g3 -O0 -W -Wall -std=gnu11 -pipe
LDFLAGS := -lpthread
EXT_CFLAG_NO_WARINGS = -Wno-int-to-pointer-cast -Wno-unused-parameter -Wno-pointer-to-int-cast -Wno-unused-variable

vpath %.h .
vpath %.c .

main: meta.c
	@echo "create Preprocess only; do not compile, to see assemble or link result..."
	gcc $(CCFLAGS) $(EXT_CFLAG_NO_WARINGS) -E $^ -o $^.preprocess.txt
	@echo "build meta programming test program ..." 
	gcc $(CCFLAGS) $(EXT_CFLAG_NO_WARINGS) $^ -o a.out $(LDFLAGS)
