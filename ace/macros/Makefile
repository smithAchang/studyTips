.PHONY: app macros

app: macros

macros: macros.cpp 
	@echo "test ACE default compiling MACROS for ACE study ..."
	g++ -g3 -O0 -W -Wall -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-unused-variable -I$(ACE_ROOT) -fpermissive $^ -o $@ -L$(ACE_ROOT)/lib -lACE
