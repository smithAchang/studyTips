.PHONY: checkstacksize
    
CCFLAGS := -g3 -O0 -W -Wall -std=gnu11 -pipe -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-variable -Wno-unused-but-set-variable
LDFLAGS := 

checkstacksize: main.c
	@echo "test stack size limits ..."
	gcc $(CCFLAGS) $^ -o $@ $(LDFLAGS)

