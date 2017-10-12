#for test
#
TOP_DIR        :=$(shell pwd)
ARCH           := x86
TOOLCHAIN_PATH :=
CROSS_COMPILE  := 
CFLAGS         := -Wall

ifeq ($(ARCH), "x86")
CC             := gcc
AR             := ar
endif

ifeq ($(ARCH), "arm")
CC             := $(TOOLCHAIN_PATH)/$(CROSS_COMPILE)gcc
AR             := $(TOOLCHAIN_PATH)/$(CROSS_COMPILE)ar
endif
BIN_NAME       := demo



.PHONY:all clean demo

TOPDIR   := $(shell pwd)
OBJ      := $(TOPDIR)/obj
SRC      := $(TOPDIR)/src
INC      := $(TOPDIR)/inc
LIB      := $(TOPDIR)/lib
BIN      := $(TOPDIR)/bin
ASL_PATH := /home/ubuntu/file/ASL

TOP_INC      := -I$(INC) -I$(INC)/freetype -I$(INC)/freetype/config
TOP_LIB_PWD  := -L$(LIB)
TOP_LIB_NAME := -lfreetype
TOP_LIB      := $(TOP_LIB_PWD) $(TOP_LIB_NAME)
CFLAGS       := $(TOP_INC) $(TOP_LIB)

-include $(ASL_PATH)/ASL_INCLUDE.MK

all:
	make -s -C $(SRC)
	make demo
demo:
	$(CC) $(OBJ)/*.o -o $(BIN)/$(BIN_NAME) $(CFLAGS)
lib:
clean:
	make -s -C $(SRC) clean

export ASL_PATH
export OBJ
export SRC
export INC
export LIB
export CC
export CFLAGS
export TOP_INC
export TOP_LIB_PWD
export TOP_LIB_NAME
export TOP_LIB
