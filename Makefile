CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm

compiler: main.c scanner.c string.c parser.c symtable.c expression.c stack.c
	$(CC) $(CFLAGS) -o $@ main.c scanner.c string.c parser.c symtable.c expression.c stack.c

clean:
	rm -f *.o compiler
