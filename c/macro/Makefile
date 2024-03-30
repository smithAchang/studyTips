.PHONY: app macro
app:  macro

vpath %.h .
vpath %.c .

macro: macro.c
	@echo "create Preprocess only; do not compile, assemble or link result..."
	gcc -g3 -O0 -W -Wall -std=gnu11 -pipe -Wno-int-to-pointer-cast -Wno-unused-parameter -Wno-pointer-to-int-cast -Wno-unused-variable -E $^ -o $^.preprocess.txt
	@echo "build macro test program ..." 
	gcc -g3 -O0 -W -Wall -std=gnu11 -pipe -Wno-int-to-pointer-cast -Wno-unused-parameter -Wno-pointer-to-int-cast -Wno-unused-variable $^ -o $@ 
