assembler: tables.o first-move.o second-move.o files.o main.o
	gcc -g -ansi -Wall -pedantic tables.o first-move.o second-move.o files.o main.o -o assembler -lm


tables.o: tables.c tables.h
	gcc -c -ansi -Wall -pedantic tables.c -o tables.o

first-move.o: first-move.c first-move.h
	gcc -c -ansi -Wall -pedantic first-move.c -o first-move.o

second-move.o: second-move.c second-move.h
	gcc -c -ansi -Wall -pedantic second-move.c -o second-move.o -lm

files.o: files.c files.h
	gcc -c -ansi -Wall -pedantic files.c -o files.o

main.o: main.c main.h
	gcc -c -ansi -Wall -pedantic main.c -o main.o
