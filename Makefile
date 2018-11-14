CC = gcc
INSTALL = install
RM = rm

CFLAGS += -Wall
PREFIX := /usr

unshare:
	$(CC) ${CFLAGS} unshare.c -o unshare

install: unshare
	$(INSTALL) -D -o 0 -g 0 -m 755 unshare ${PREFIX}/bin/unshare

clean:
	$(RM) -rf *.o unshare

all: unshare
