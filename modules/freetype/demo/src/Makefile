MODULE_SRCS := $(wildcard *.c)
MODULE_OBJS := $(MODULE_SRCS:%.c=%.o)
MODULE_DEPS := $(MODULE_OBJS:%.o=%.d)

.PHONY:all clean

MODULE_INC      := -I./ $(TOP_INC)
MODULE_LIB_PWD  := $(TOP_LIB_PWD)
MODULE_LIB_NAME := $(TOP_LIB_NAME)
MODULE_LIB      := $(MODULE_LIB_PWD) $(MODULE_LIB_NAME)
CFLAGS          := $(MODULE_INC) $(MODULE_LIB)


all:$(MODULE_OBJS) 
	mv $(MODULE_OBJS) $(OBJ)

%.d:%.c
	@set -e;rm -f $@;\
	$(CC) -MM $(CFLAGS) $< > $@.temp;\
	sed 's,\($*\)\.o[:]*,\1.o $@:,g' < $@.temp > $@;\
	rm -f $@.temp;	
-include $(MODULE_DEPS)

clean:
	rm *.d *.o -f

