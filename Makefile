PRJ=compiler
#
PROGS=$(PRJ)
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm

all: $(PROGS)

$(PRJ): main.c scanner.c
	$(CC) $(CFLAGS) -o $@ main.c scanner.c

clean:
	rm -f *.o *.out $(PROGS)