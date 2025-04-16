.PHONY: app main
app: main

vpath %.h .
vpath %.cpp .

CCFLAGS := -g3 -O0 -W -Wall -std=gnu++11 -pipe -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-int-to-pointer-cast
LDFLAGS := -lpthread

main: main.cpp
	@echo "test thread local storage ..."
	g++ $(CCFLAGS) $^ -o a.out $(LDFLAGS)
