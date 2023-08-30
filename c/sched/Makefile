.PHONY: app sched
app: sched

NO_WARNINGS = -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-variable -Wno-unused-parameter
NO_WARNINGS += -Wno-unused-value

vpath %.h .
vpath %.c .

sched: sched.c
	@echo "build bits field increased beyond the threshold test program ..."
	gcc -g3 -O0 -W -Wall $(NO_WARNINGS) $^
