all:    compiler.o

compiler.o : compiler.c
	        gcc -ansi -pedantic -Wall -g *.c -o compiler.o -lm
