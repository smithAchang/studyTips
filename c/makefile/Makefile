# Demo shows var usages in makefile

$(info "inherit envrionment var from host, PATH: $(PATH) ...")

$(info "BIN var from make command parameter, BIN : $(BIN) ...")

BIN ?= demo

$(info "Set default value of BIN : $(BIN) ...")

simplevar = a 
$(info "After set var using = operator, value: $(simplevar)")

simplevar += b
$(info "After set var using += operator, value: $(simplevar)")

CONST_HUMAN_TIME := $(shell date +%Y-%m-%d_%H-%M-%S)
$(info "CONST_HUMAN_TIME: $(CONST_HUMAN_TIME)")

CONST_START_TIME := $(shell date +%s)
NOW_TIME     = $(shell date +%s)
$(info "CONST_START_TIME: $(CONST_START_TIME)")

$(shell sleep 1)

$(info "NOW_TIME: $(NOW_TIME)")
$(shell sleep 2)

VAR_CURRENT_TIME = $(shell date +%Y-%m-%d_%H-%M-%S)
$(info "VAR_CURRENT_TIME: $(VAR_CURRENT_TIME)")

$(shell sleep 3)
$(info "VAR_CURRENT_TIME after sleep: $(VAR_CURRENT_TIME)")

OBJS = main.o
OBJS += foo.o

.PHONY: all clean $(OBJS)
all: clean $(OBJS)
	$(CC) $(OBJS) -o $(BIN)
	@echo "Elapsed time: $(shell expr $(NOW_TIME) - $(CONST_START_TIME)) seconds"

# 声明依赖关系
$(OBJS): clean

# 静态模糊匹配规则
$(OBJS): %.o: %.c
	$(CC) -c $< -o $@

clean:
	$(RM) *.o $(BIN)
