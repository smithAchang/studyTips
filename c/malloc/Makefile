.PHONY: app main
app: main

CCFLAGS := -g3 -O0 -W -Wall -std=gnu11 -pipe -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-variable -Wno-unused-but-set-variable
LDFLAGS := -lpthread

vpath %.h .
vpath %.c .

main: main.c
	@echo "to see VRIT & RES memory stat ..."
	gcc $(CCFLAGS) $^ $(LDFLAGS)
