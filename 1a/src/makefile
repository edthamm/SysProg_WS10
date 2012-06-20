CC=gcc
CFLAGS= -ansi -pedantic -Wall -D_XOPEN_SOURCE=500 -D_BSD_SOURCE


all: palindrom

palindrom: palindrom.c
	$(CC) $(CFLAGS) -o palindrom palindrom.c

clean:
	-rm -f palindrom
