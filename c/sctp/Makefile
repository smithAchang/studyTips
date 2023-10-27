.PHONY: checksctp

checksctp: main.c
	@echo "test sctp support using sock API ..."
	gcc -g3 -O0 -W -Wall -std=gnu11 -pipe -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-variable -Wno-unused-but-set-variable $^ -o $@
