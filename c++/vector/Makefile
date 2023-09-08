.PHONY: app main
app: main

vpath %.h .
vpath %.cpp .

main: main.cpp
	@echo "build vector no-implicit-templates test program ..."
	g++ -g3 -O0 -W -Wall -pipe -Wno-unused-variable -Wno-unused-parameter -fno-implicit-templates -lstdc++ $^ 
