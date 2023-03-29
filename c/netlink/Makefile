.PHONY: app getroute
app: getroute

vpath %.h .
vpath %.c .

getroute: getroute.c
	@echo "create Preprocess only; do not compile, assemble or link result..."
	gcc -g3 -O0 -W -Wall -Wno-int-to-pointer-cast -Wno-unused-parameter -Wno-pointer-to-int-cast -Wno-unused-variable -E $^ -o $@.preprocess.txt
	@echo "build getroute program ..."
	gcc -g3 -O0 -W -Wall -Werror=nonnull -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-variable $^ -o $@ 
