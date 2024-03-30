.PHONY: app bitincr
app: bitincr

vpath %.h .
vpath %.c .

bitincr: bitincr.c
	@echo "build bits field increased beyond the threshold test program ..."
	gcc -g3 -O0 -W -Wall -std=gnu11 -pipe -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-variable -Wno-unused-parameter $^ -o $@ 
