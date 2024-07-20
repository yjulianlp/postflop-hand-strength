CC = gcc
CFLAGS = -Wall -Wextra -Werror
BUILD_FLAGS = -std=c99
DBG_FLAGS = -std=c99 -ggdb -DDEBUG
VPATH = ../include

.PHONY: build debug

build: CFLAGS += $(BUILD_FLAGS)
debug: CFLAGS += $(DBG_FLAGS)

build: main.c processinput.o handranking.o managecards.o cardinfo.o gamestate.o
	$(CC) $(CFLAGS) cardinfo.o managecards.o handranking.o processinput.o gamestate.o main.c -o main

debug: main.c processinput.o handranking.o managecards.o cardinfo.o gamestate.o
	$(CC) $(CFLAGS) cardinfo.o managecards.o handranking.o processinput.o gamestate.o main.c -o debug

cardinfo.o: gameinfo.h cardinfo.h cardinfo.c
	$(CC) $(CFLAGS) -c cardinfo.c -o cardinfo.o

managecards.o: managecards.h gameinfo.h managecards.c
	$(CC) $(CFLAGS) -c managecards.c -o managecards.o
	
processinput.o: processinput.c processinput.h gameinfo.h
	$(CC) $(CFLAGS) processinput.c -c -o processinput.o

handranking.o: gameinfo.h handranking.h handranking.c
	$(CC) $(CFLAGS) -c handranking.c -o handranking.o

gamestate.o: gamestate.h gamestate.c gameinfo.h managecards.h processinput.h
	$(CC) $(CFLAGS) -c gamestate.c -o gamestate.o
clean:
	rm -f *.o





