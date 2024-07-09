build: main.c processinput.o handranking.o managecards.o cardinfo.o
	gcc -Wall -Wextra -Werror -std=c99 cardinfo.o managecards.o handranking.o processinput.o main.c -o main

debug: main.c processinput.o handranking.o managecards.o cardinfo.o
	gcc -Wall -Wextra -Werror -std=c99 -ggdb -D DEBUG cardinfo.o managecards.o handranking.o processinput.o main.c -o debug

cardinfo.o: gameinfo.h cardinfo.h cardinfo.c
	gcc -Wall -Wextra -Werror -std=c99 -c cardinfo.c -o cardinfo.o

managecards.o: managecards.h gameinfo.h managecards.c
	gcc -Wall -Wextra -Werror -std=c99 -c managecards.c -o managecards.o
	
processinput.o: processinput.c processinput.h gameinfo.h
	gcc -Wall -Wextra -Werror -std=c99 processinput.c -c -o processinput.o

handranking.o: gameinfo.h handranking.h handranking.c
	gcc -Wall -Wextra -Werror -std=c99 -c handranking.c -o handranking.o





