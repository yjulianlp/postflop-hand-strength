CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99
VPATH = ../include

build: main.c processinput.o handranking.o managecards.o cardinfo.o
	$(CC) $(CFLAGS) cardinfo.o managecards.o handranking.o processinput.o main.c -o main

debug: CFLAGS = -Wall -Wextra -Werror -std=c99 -D DEBUG

debug: main.c processinput.o handranking.o managecards.o cardinfo.o
	$(CC) $(CFLAGS) -ggdb cardinfo.o managecards.o handranking.o processinput.o main.c -o debug

cardinfo.o: gameinfo.h cardinfo.h cardinfo.c
	$(CC) $(CFLAGS) -c cardinfo.c -o cardinfo.o

managecards.o: managecards.h gameinfo.h managecards.c
	$(CC) $(CFLAGS) -c managecards.c -o managecards.o
	
processinput.o: processinput.c processinput.h gameinfo.h
	$(CC) $(CFLAGS) processinput.c -c -o processinput.o

handranking.o: gameinfo.h handranking.h handranking.c
	$(CC) $(CFLAGS) -c handranking.c -o handranking.o

clean:
	rm -f *.o





