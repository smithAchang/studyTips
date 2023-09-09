# big file for explicit template instantiation 
#

.PHONY: app main clean cleanObjs
app: main

vpath %.h .
vpath %.cpp .

SRC=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o, $(SRC))
TARGET=a.out

CCFLAGS:=-g3 -O0 -W -Wall -pipe -Wno-unused-variable -Wno-unused-parameter -fno-implicit-templates 
LDFLAGS:=-lstdc++

main: $(OBJS)
	@echo "Link vector no-implicit-templates test program ..."
	g++ -pipe $(OBJS) -o $(TARGET) $(LDFLAGS)

# depend & nested
$(OBJS): cleanObjs
tmplinst.o: tmplinst.cpp
	@echo "Build $@ specially for implicit templates instantiation by removing -fno-implicit-templates compiler option ..."
	g++ $(strip $(subst -fno-implicit-templates, , $(CCFLAGS))) -c $< -o $@
%.o: %.cpp
	@echo "Build $< to $@ with -fno-implicit-templates compiler option ..."
	g++ $(CCFLAGS) -c $< -o $@
	
clean: 
	@echo "Clean all obj & target ..."	
	rm -rf $(OBJS) $(TARGET)
	@echo "Finish clean all obj & target ..."	

cleanObjs:
	@echo "Clean all objs ..."	
	rm -rf $(OBJS)
	@echo "Finish clean all objs ..."	
