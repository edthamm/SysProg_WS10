/**
@file makefile
@author Eduard Thamm 0525087
@brief makefile for caeser project
@date 19.5.11
*/

CFLAGS = -ansi -pedantic -Wall -D_XOPEN_SOURCE=500 -D_BSD_SOURCE -g
CC = gcc

all: caesarproject

caesarproject: readin caesar

caesar: caesar.c
	$(CC) $(CFLAGS) -o caesar caesar.c

readin: readin.c
	$(CC) $(CFLAGS) -o readin readin.c
	
