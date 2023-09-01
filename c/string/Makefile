.PHONY: app string_compile_cat
app: string_compile_cat

vpath %.h .
vpath %.c .

string_compile_cat: string_compile_cat.c
	@echo "create Preprocess only; do not compile, assemble or link result..."
	gcc -g3 -O0 -W -Wall -std=gnu11 -pipe -Wno-int-to-pointer-cast -Wno-unused-parameter -Wno-pointer-to-int-cast -Wno-unused-variable -E $^ -o $@.preprocess.txt 
	@echo "build string_compile_cat usage test program ..."
	gcc -g3 -O0 -W -Wall -std=gnu11 -pipe -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-variable -Wno-unused-parameter $^ -o $@ 
