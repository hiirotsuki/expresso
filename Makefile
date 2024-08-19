CC ?= gcc
CFLAGS ?= -Os
LDFLAGS ?= -nostdlib -s
LIBS ?= -lkernel32

all: expresso tray

expresso: expresso.c
	$(CC) $(CFLAGS) $(LDFLAGS) expresso.c -o expresso $(LIBS)

tray: tray.c
	$(CC) $(CFLAGS) $(LDFLAGS) tray.c -o tray $(LIBS) -luser32 -lshell32
