CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm -ggdb3

compiler: main.c scanner.c string.c parser.c symtable.c expression.c stack.c generator.c
	$(CC) $(CFLAGS) -o $@ main.c scanner.c string.c parser.c symtable.c expression.c stack.c generator.c

clean:
	rm -f *.o compiler
