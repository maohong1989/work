TOPDIR   := $(shell pwd)/..
OBJ      := $(TOPDIR)/obj
SRC      := $(TOPDIR)/src
INC      := $(TOPDIR)/inc
LIB      := $(TOPDIR)/lib
BIN      := $(TOPDIR)/bin
TOP_INC      := -I$(INC) -I$(INC)/freetype -I$(INC)/freetype/config -I./
TOP_LIB_PWD  := -L$(LIB)
TOP_LIB_NAME := -lfreetype
TOP_LIB      := $(TOP_LIB_PWD) $(TOP_LIB_NAME)
CFLAGS       := $(TOP_INC) $(TOP_LIB)
all:
	gcc ./*.c $(CFLAGS)
