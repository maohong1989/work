####################################################################################
##                        版权所有，作者保留一切权利
## 但保证本程序完整性（包括版权申明，作者信息）的前提下，欢迎任何人对此进行修改传播
## 作者邮箱：apostle9891@foxmail.com，欢迎进行交流，请勿用于商业用途
####################################################################################
## 文 件 名: ASL_Rules.make
## 作    者: apostle --- apostle9891@foxmail.com
## 版    本: version 1.0
## 日    期: 2013-12-06 10:55
## 描    述:
## 历史描述:
####################################################################################
# [目录]
# --行数--- [目录]
# --24----- 1 [配置选项]
# --25-----   1.1 [配置选项: BUILD    |COMBINE--编译成lib |PART--编译成单个lib]
# --26-----   1.2 [配置选项: BUILD_EXE|YES--表示会编译成可执行文件|NO--表示不编译成可执行文件]
# --27-----   1.3 [配置选项: VALID    |@--编译显示隐藏]
# --34----- 2 [基本设置,需配置]
# --39----- 3 [基本设置,勿需配置]
# --47----- 4 [设置常用环境变量]
# --78----- 5 [export申明]
####################################################################################
#=@_@=#  配置选项
#=@__@=# 配置选项: BUILD    |COMBINE--编译成lib |PART--编译成单个lib
#=@__@=# 配置选项: VALID    |@--编译显示隐藏
# BUILD     ?= COMBINE
# BUILD_EXE ?= NO
BUILD       ?= PART
VALID        = @
# VALID        =
####################################################################################
#=@_@=# 基本设置,需配置
ASL_BASE_PATH    := /home/deepglint/Nutstore/ASL_SDK/code
TOOLCHAIN_PATH   := /usr/bin
CROSS_COMPILE    :=
####################################################################################
#=@_@=# 基本设置,勿需配置
ASL_BASE_LIB     := $(ASL_BASE_PATH)/lib
ASL_BASE_INC     := $(ASL_BASE_PATH)/inc
ASL_BASE_OBJ     := $(ASL_BASE_PATH)/obj
ASL_BASE_MODULES := $(ASL_BASE_PATH)/modules

INC_PATH          = -I $(ASL_BASE_INC)
####################################################################################
#=@_@=# 设置常用环境变量
####################################################################################
CC                  = $(TOOLCHAIN_PATH)/$(CROSS_COMPILE)gcc
CXX                 = $(TOOLCHAIN_PATH)/$(CROSS_COMPILE)g++
STRIP               = $(TOOLCHAIN_PATH)/$(CROSS_COMPILE)strip
AR                  = $(TOOLCHAIN_PATH)/$(CROSS_COMPILE)ar
# MAKE               := $(MAKE) -s
# CFLAGS              = -Wall -O2
# CXXFLAGS            = -Wall -O2
MAKE               := $(MAKE)
CFLAGS              = -g
CXXFLAGS            = -g

AR_CFG              = -rcs
LEX                 = flex
YACC                = yacc
TAR                 = tar -cf
GZIP                = gzip -9f
COPY                = cp -f
COPY_FILE           = $(COPY)
COPY_DIR            = $(COPY) -r
INSTALL_FILE        = $(COPY_FILE)
INSTALL_DIR         = $(COPY_DIR)
DEL_FILE            = rm -f
SYMLINK             = ln -sf
DEL_DIR             = rmdir
MOVE                = mv -f
CHK_DIR_EXISTS      = test -d
MKDIR               = mkdir -p
INSTALL             = install -c
####################################################################################
#=@_@=# export申明
####################################################################################
export BUILD
export BUILD_EXE
export VALID
export ASL_BASE_PATH
export TOOLCHAIN_PATH
export CROSS_COMPILE
export ASL_BASE_LIB
export ASL_BASE_INC
export ASL_BASE_OBJ
export ASL_BASE_MODULES
export INC_PATH
export CC
export CXX
export STRIP
export AR
export MAKE
export CFLAGS
export CXXFLAGS
export AR_CFG
export LEX
export YACC
export TAR
export GZIP
export COPY
export COPY_FILE
export COPY_DIR
export INSTALL_FILE
export INSTALL_DIR
export DEL_FILE
export SYMLINK
export DEL_DIR
export MOVE
export CHK_DIR_EXISTS
export MKDIR
export INSTALL
