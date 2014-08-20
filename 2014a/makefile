assembler: assembler.o hashtable.o parse.o utils.o data.o
	gcc -g -ansi -Wall assembler.o hashtable.o parse.o utils.o data.o -o assembler

assembler.o: assembler.c assembler.h hashtable.h parse.h data.h
	gcc -c assembler.c -ansi -Wall -o assembler.o

hashtable.o: hashtable.c hashtable.h utils.h
	gcc -c hashtable.c -ansi -Wall -o hashtable.o

parse.o: parse.c parse.h utils.h data.h hashtable.h
	gcc -c parse.c -ansi -Wall -o parse.o

utils.o: utils.c utils.h
	gcc -c utils.c -ansi -Wall -o utils.o

data.o: data.c data.h
	gcc -c data.c -ansi -Wall -o data.o
