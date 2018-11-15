CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm

compiler: main.c scanner.c string.c parser.c
	$(CC) $(CFLAGS) -o $@ main.c scanner.c string.c parser.c

clean:
	rm -f *.o compiler
