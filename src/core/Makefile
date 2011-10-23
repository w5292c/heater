include $(TOPDIR)/config.mk

LIB	:= $(obj)libi2c.a

COBJS-y += editor.o
COBJS-y += engine.o
COBJS-y += fuses.o
COBJS-y += scheduler.c

COBJS	:= $(COBJS-y)
SRCS	:= $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS))

all:	$(LIB)
