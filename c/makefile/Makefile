# Demo shows var usages in makefile
# 约定：+ 强烈建议使用make定义的内置变量和自动变量，以利于可读性和规范性
# 约定：+ 变量使用大写
# 约定：+ 可选参数需要通过条件变量提供默认处理
# 约定：+ 内部变量以下划线开头命名

$(info "inherit envrionment var from host, PATH: $(PATH) ...")

# 通用做法，通过内置变量定制编译选项
CFLAGS := -Wall -W -g -pipe -Wno-unused-parameter
ifeq ($(DEBUG_VERSION), yes)
#   变量转编译器可识别宏定义
    CFLAGS += -O0 -D_DEBUG
else
    CFLAGS += -O2
endif

$(info "BIN var from make command optional parameter, BIN : $(BIN) ...")
BIN ?= demo

$(info "Try to set default value for BIN : $(BIN) ...")

simplevar = a 
$(info "After set var using = operator, value: $(simplevar)")

simplevar += b
$(info "After set var using += operator, value: $(simplevar)")

VAR_CURRENT_TIME = $(shell date +%Y-%m-%d_%H-%M-%S)
CONST_HUMAN_TIME := $(shell date +%Y-%m-%d_%H-%M-%S)
$(info "CONST_HUMAN_TIME using := operator : $(CONST_HUMAN_TIME)")

$(shell sleep 1)

CONST_START_TIME := $(shell date +%s)
NOW_TIME     = $(shell date +%s)
$(info "CONST_START_TIME using := operator : $(CONST_START_TIME)")

$(shell sleep 1)

$(info "NOW_TIME: $(NOW_TIME)")
$(info "VAR_CURRENT_TIME: $(VAR_CURRENT_TIME)")

$(shell sleep 3)
$(info "VAR_CURRENT_TIME after sleep: $(VAR_CURRENT_TIME)")

vpath %.c .

# 使用模糊匹配
SRCS := $(wildcard *.c)

# 简单文本替换
OBJS := $(SRCS:.c=.o)


.PHONY: $(BIN) clean $(OBJS)
# 自顶向下为并行编译声明依赖关系
$(BIN): $(OBJS)
$(OBJS): clean

# 建议使用自动变量
$(BIN):
	$(CC) $^ -o $@ $(LDFLAGS)
	@echo "Elapsed time: $(shell expr $(NOW_TIME) - $(CONST_START_TIME)) seconds"

# 静态模糊匹配规则
$(OBJS): %.o: %.c
	@echo "Begin compile $< ..."
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Has finished compiling $<"

clean:
	@echo "$@ target begin clean old resources ..."
	$(RM) *.o $(BIN)
	@echo "$@ target has finished clean old resources"