build: main.c processinput.o
	gcc -Wall -Wextra -Werror -std=c99 processinput.o main.c -o main

debug: main.c processinput.o
	gcc -Wall -Wextra -Werror -std=c99 -ggdb -D DEBUG processinput.o main.c -o debug

processinput.o: processinput.c processinput.h
	gcc -Wall -Wextra -Werror -std=c99 -c processinput.c -o processinput.o
