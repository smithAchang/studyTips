.PHONY: app main
app: main

vpath %.h .
vpath %.cpp .

SRC=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp, %.o, $(SRC))
TARGET=a.out

.PHONY: clean main


CCFLAGS:=-g3 -O0 -W -Wall -pipe -Wno-unused-variable -Wno-unused-parameter -fno-implicit-templates 

main: $(OBJS)
	@echo "build vector no-implicit-templates test program ..."
	g++ $(OBJS) -o $(TARGET)

# basic units
tmplinst.o:
	g++ $(strip $(subst -fno-implicit-templates, , $(CCFLAGS))) -c $(subst .o,.cpp, $@) -o $@

%.o: %.cpp
	g++ $(CCFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS) $(TARGET)

