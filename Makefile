CC=gcc

CFLAGS += -std=c99
LDFLAGS += `pkg-config --libs x11`

SRCS  = gwm.c
SRCS += gwm_context.c
SRCS += gwm_window.c
SRCS += gwm_focus_raise.c
HDRS  = $(SRCS,.c=.h)
OBJS  = $(SRCS:.c=.o)

all: gwm

gwm: $(OBJS)
