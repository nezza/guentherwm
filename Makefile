CC=gcc

CFLAGS += -std=c99 -Wall -pedantic
LDFLAGS += `pkg-config --libs x11`

SRCS  = gwm.c
SRCS += gwm_context.c
SRCS += gwm_window.c
SRCS += gwm_focus_raise.c
SRCS += gwm_functions.c
SRCS += gwm_keys.c
SRCS += gwm_workspace.c
SRCS += gwm_tiling.c
HDRS  = $(SRCS,.c=.h)
OBJS  = $(SRCS:.c=.o)

all: gwm

gwm: $(OBJS)

clean:
	rm gwm $(OBJS)

