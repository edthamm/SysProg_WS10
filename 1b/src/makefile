CFLAGS = -ansi -pedantic -Wall -D_XOPEN_SOURCE=500 -D_BSD_SOURCE 

all: dicebot

dicebot: dicebot.c
	gcc $(CFLAGS) -o dicebot dicebot.c

clean:
	-rm -f dicebot
