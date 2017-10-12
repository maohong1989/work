####################################################################################
##                        版权所有，作者保留一切权利
## 但保证本程序完整性（包括版权申明，作者信息）的前提下，欢迎任何人对此进行修改传播
## 作者邮箱：apostle9891@foxmail.com，欢迎进行交流，请勿用于商业用途
####################################################################################
## 文 件 名: Makefile
## 作    者: apostle --- apostle9891@foxmail.com
## 版    本: version 1.0
## 日    期: 2013-12-06 11:13
## 描    述:
## 历史描述:
####################################################################################
include ../ASL_Rules.make
.PHONY: all part clean
# SUB_DIRS = time
SUB_DIRS = $(shell ls -F | grep /)
all:
	${VALID} for i in $(SUB_DIRS); do $(MAKE) -C $$i ; done
clean:
	${VALID} for i in $(SUB_DIRS); do $(MAKE) -C $$i clean; done
