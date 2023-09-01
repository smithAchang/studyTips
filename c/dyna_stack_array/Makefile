.PHONY: app dynaArray
app: dynaArray

vpath %.h .
vpath %.c .

dynaArray: dynaArray.c
	@echo "build dynamic stack array usage program ..."
	gcc -g3 -O0 -W -Wall -std=gnu11 -pipe -Wno-int-to-pointer-cast -Wno-missing-field-initializers -Wno-unused-but-set-variable -Wno-pointer-to-int-cast -Wno-unused-variable -Wno-unused-parameter $^ -o $@ 
