####################################################################################
##                        版权所有，作者保留一切权利
## 但保证本程序完整性（包括版权申明，作者信息）的前提下，欢迎任何人对此进行修改传播
## 作者邮箱：apostle9891@foxmail.com，欢迎进行交流，请勿用于商业用途
####################################################################################
## 文 件 名: Makefile
## 作    者: apostle --- apostle9891@foxmail.com
## 版    本: version 1.1
## 日    期: 2013-12-06 11:13
## 描    述: Makefile通用版本
## 历史描述: 1. 1.0版本升级，加入注释等
####################################################################################
include ../../ASL_Rules.make
.PHONY: all clean
####################################################################################
# [目录]
# --行数--- [目录]
# --32----- 1 [基础配置，选择文件]
# --34-----   1.1 [模块假如有文件夹，模块复杂，用多文件格式]
# --53-----   1.2 [假如为单模块，将库和obj放在当前，否则放在相应SDK包处]
# --60----- 2 [基础配置，勿需更改]
# --62-----   2.1 [设置模块的基本参数]
# --69----- 3 [inc 配置]
# --74----- 4 [编译]
# --76-----   4.1 [all 编译]
# --89-----   4.2 [BIG_MODULE 编译]
# --92-----   4.3 [lib 编译]
# --96-----   4.4 [obj 编译]
# --100----   4.5 [exe 编译]
# --108----   4.6 [clean 编译]
####################################################################################
#=@_@=# 基础配置，选择文件
####################################################################################
#=@__@=# 模块假如有文件夹，模块复杂，用多文件格式
CUR_PATH  := $(shell pwd)
CUR_COMPLEX  = $(shell ls -F | grep obj/)
ifeq ($(CUR_COMPLEX),)
	CUR_NAME      := $(notdir $(CUR_PATH))
	CUR_DEMO_PATH := $(CUR_PATH)
	CUR_INC_PATH  := $(CUR_PATH)
	CUR_SRC_PATH  := $(CUR_PATH)
	CUR_LIB_PATH  := $(CUR_PATH)
	CUR_OBJ_PATH  := $(CUR_PATH)
else
	CUR_NAME      := $(notdir $(CUR_PATH))
	CUR_DEMO_PATH := $(CUR_PATH)/demo
	CUR_INC_PATH  := $(CUR_PATH)/inc
	CUR_SRC_PATH  := $(CUR_PATH)/src
	CUR_LIB_PATH  := $(CUR_PATH)/lib
	CUR_OBJ_PATH  := $(CUR_PATH)/obj
endif

#=@__@=# 假如为单模块，将库和obj放在当前，否则放在相应SDK包处
ifeq ($(BUILD), PART)
	TARGET_OBJ_PATH := $(CUR_OBJ_PATH)
else
	TARGET_OBJ_PATH := $(ASL_BASE_OBJ)
endif
####################################################################################
#=@_@=# 基础配置，勿需更改
####################################################################################
#=@__@=# 设置模块的基本参数
LIB_NAME         := lib$(CUR_NAME).a
SRC_NAME         := $(wildcard $(CUR_SRC_PATH)/*.c)
INC_NAME         := $(wildcard $(CUR_SRC_PATH)/*.h)
SRC_NAME         := $(notdir $(SRC_NAME))
INC_NAME         := $(notdir $(INC_NAME))
ASL_BASE_INC_NAME:= ${addprefix ${ASL_BASE_INC}/, ${INC_NAME}}
OBJ_NAME         := $(patsubst %.c,%.o,$(SRC_NAME))
TARGET_DEMO_NAME := $(CUR_NAME)_demo
####################################################################################
#=@_@=# inc 配置
####################################################################################
INC_PATH         += -I $(ASL_BASE_INC)/hash
LIB_PATH          = -L$(ASL_BASE_LIB) 
CFLAGS           += $(INC_PATH)
LIBFLAGS         += $(LIB_PATH)
####################################################################################
#=@_@=# 编译
####################################################################################
#######=@__@=# all 编译#########################
all:${ASL_BASE_INC_NAME}
	${VALID} echo -e "\033[34mcompiling the modules [$(CUR_NAME)]...\033[0m"
ifeq ($(BUILD), PART)
	${VALID} echo -e "\033[34mcompiling the $(LIB_NAME)...\033[0m"
	${VALID} $(MAKE) $(LIB_NAME)
	${VALID} $(COPY) $(LIB_NAME) $(ASL_BASE_LIB)
else
	${VALID} echo -e "\033[34mcompiling the BIG_MODULE...\033[0m"
	${VALID} $(MAKE) BIG_MODULE
endif
	${VALID} ${MAKE} exe

#######=@__@=# inc 编译#########################
#假如公共inc中的头文件没有当下的新，则替换过去
${ASL_BASE_INC_NAME}:${ASL_BASE_INC}/%:${CUR_INC_PATH}/%
	${VALID} echo -e "${INC_NAME} change"
	cp $< $@

#######=@__@=# BIG_MODULE 编译##################
BIG_MODULE:$(TARGET_OBJ_PATH)/$(OBJ_NAME)

#######=@__@=# lib 编译#########################
$(LIB_NAME):$(TARGET_OBJ_PATH)/$(OBJ_NAME)
	$(AR) $(AR_CFG) $(LIB_NAME) $(TARGET_OBJ_PATH)/$(OBJ_NAME)

#######=@__@=# obj 编译#########################
vpath %.c $(CUR_SRC_PATH)
$(TARGET_OBJ_PATH)/%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
#######=@__@=# exe 编译#########################
exe:
	${VALID} echo -e "\033[34mcompiling the demo    [$(CUR_NAME)]...\033[0m"
ifeq ($(CUR_COMPLEX),)
	$(CC) $(CFLAGS) $(CUR_OBJ_PATH)/*.c ${LIBFLAGS} -o $(CUR_DEMO_PATH)/$(TARGET_DEMO_NAME)
else
	$(CC) $(CFLAGS) $(CUR_DEMO_PATH)/*.c ${LIBFLAGS} $(CUR_OBJ_PATH)/*.o -o $(CUR_DEMO_PATH)/$(TARGET_DEMO_NAME)
endif
#######=@__@=# clean 编译#######################
clean:
	${VALID} echo -e "\033[34mclean $(CUR_NAME)...\033[0m"
	-rm -rf $(CUR_OBJ_PATH)/*.o
	-rm -rf $(CUR_DEMO_PATH)/$(TARGET_DEMO_NAME)
	-rm -rf $(LIB_NAME)
