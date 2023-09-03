.PHONY: app iovec

app: iovec

iovec: iovec.cpp 
	@echo "test udp dgram iovec program for ACE study ..."
	g++ -g3 -O0 -W -Wall -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-unused-variable -I$(ACE_ROOT) -fpermissive $^ -o $@ -lACE
