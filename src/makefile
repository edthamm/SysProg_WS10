#/**
#@file makefile
#@author Eduard Thamm 0525087
#@brief makefile for caeser project
#@date 19.5.11
#*/

CFLAGS = -ansi -pedantic -Wall -D_XOPEN_SOURCE=500 -D_BSD_SOURCE -g
CC = gcc
LL = -lsem182

all: caesarproject

caesarproject: readin caesar

caesar: caesar.c common.h common.o
	$(CC) $(CFLAGS) -o caesar caesar.c common.o $(LL)
readin: readin.c common.h common.o
	$(CC) $(CFLAGS) -o readin readin.c common.o $(LL)
	
common.o: common.c common.h
	$(CC) $(CFLAGS) -c -o common.o common.c

clean: 
	-rm -f *.o readin caesar
