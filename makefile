build: main.c processinput.o handranking.o
	gcc -Wall -Wextra -Werror -std=c99 handranking.o processinput.o main.c -o main

debug: main.c processinput.o handranking.o
	gcc -Wall -Wextra -Werror -std=c99 -ggdb -D DEBUG handranking.o processinput.o main.c -o debug

processinput.o: processinput.c processinput.h gameinfo.h handranking.o
	gcc -Wall -Wextra -Werror -std=c99 processinput.c -c -o processinput.o

handranking.o: gameinfo.h handranking.h
	gcc -Wall -Wextra -Werror -std=c99 -c handranking.c -o handranking.o
