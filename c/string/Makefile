.PHONY: app string_compile_cat
app: string_compile_cat

CCFLAGS := -g3 -O0 -W -Wall -std=gnu11 -pipe
LDFLAGS := -lpthread

EXT_CFLAG_NO_WARINGS =  -Wno-int-to-pointer-cast -Wno-unused-parameter -Wno-pointer-to-int-cast -Wno-unused-variable
EXT_CFLAG_NO_WARINGS += -Wno-unused-function

vpath %.h .
vpath %.c .

string_compile_cat: string_compile_cat.c
	@echo "create Preprocess only; do not compile, assemble or link result..."
	gcc $(CCFLAGS) $(EXT_NO_WARNINGS) -E $^ -o $^.preprocess.txt 
	@echo "build string_compile_cat usage test program ..."
	gcc $(CCFLAGS) $(EXT_NO_WARNINGS) $^ -o $@ $(LDFLAGS)
