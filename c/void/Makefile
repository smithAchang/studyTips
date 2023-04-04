.PHONY: app voidptr
app: voidptr

vpath %.h .
vpath %.c .

voidptr: voidptr.c
	@echo "build void* ptr usage test program ..."
	gcc -g3 -O0 -W -Wall -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-variable -Wno-unused-parameter $^ -o $@ 
