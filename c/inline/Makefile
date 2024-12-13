.PHONY: inlineTest

CCFLAGS := -g3 -O0 -W -Wall -std=gnu11 -pipe -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter
LDFLAGS := -lpthread

doubtTest: main.c inline.c
	@echo "test inline feature ..."
	gcc $(CCFLAGS) $^ $(LDFLAGS)
