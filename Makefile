CC ?= gcc
CFLAGS ?= -Os
LDFLAGS ?= -nostdlib -s
LIBS ?= -lkernel32

expresso: expresso.c
	$(CC) $(CFLAGS) $(LDFLAGS) expresso.c -o expresso $(LIBS)
