CFLAGS = -ansi -pedantic -Wall -D_XOPEN_SOURCE=500 -D_BSD_SOURCE -g
CC = gcc

all: dsort

dsort: dsort.c
	$(CC) $(CFLAGS) -o dsort dsort.c

clean: 
	rm -f dsort
