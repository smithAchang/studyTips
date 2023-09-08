# suport
#

.PHONY: app main
app: main

vpath %.h .
vpath %.cpp .

SRC=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp, %.o, $(SRC))
TARGET=a.out

.PHONY: clean cleanObjs

CCFLAGS:=-g3 -O0 -W -Wall -pipe -Wno-unused-variable -Wno-unused-parameter -fno-implicit-templates 

main: $(OBJS)
	@echo "Link vector no-implicit-templates test program ..."
	g++ $(OBJS) -o $(TARGET)

# depend & nested
$(OBJS): cleanObjs
tmplinst.o:
	@echo "Build $@ specially ..."
	g++ $(strip $(subst -fno-implicit-templates, , $(CCFLAGS))) -c $(subst .o,.cpp, $@) -o $@
%.o: %.cpp
	@echo "Build $< to $@ ..."
	g++ $(CCFLAGS) -c $< -o $@
	
clean: 
	@echo "Clean all obj & target ..."	
	rm -rf $(OBJS) $(TARGET)
	@echo "Finish clean all obj & target ..."	

cleanObjs:
	@echo "Clean all objs ..."	
	rm -rf $(OBJS)
	@echo "Finish clean all objs ..."	
