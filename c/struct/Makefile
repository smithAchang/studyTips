.PHONY: test


test: main.c
	@echo "create Preprocess only; do not compile, assemble or link result..."
	gcc -g3 -O0 -W -Wall -Wno-int-to-pointer-cast -Wno-unused-parameter -Wno-pointer-to-int-cast -Wno-unused-variable -E $^ -o $@.preprocess.txt 
	@echo "feature test program ..."
	gcc -g3 -O0 -W -Wall -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-variable -Wno-unused-but-set-variable $^ 
