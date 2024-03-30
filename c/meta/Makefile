.PHONY: app main
app:  main

vpath %.h .
vpath %.c .

main: meta.c
	@echo "create Preprocess only; do not compile, to see assemble or link result..."
	gcc -g3 -O0 -W -Wall -std=gnu11 -pipe -Wno-int-to-pointer-cast -Wno-unused-parameter -Wno-pointer-to-int-cast -Wno-unused-variable -E $^ -o $^.preprocess.txt
	@echo "build meta programming test program ..." 
	gcc -g3 -O0 -W -Wall -std=gnu11 -pipe -Wno-int-to-pointer-cast -Wno-unused-parameter -Wno-pointer-to-int-cast -Wno-unused-variable $^ -o a.out 
