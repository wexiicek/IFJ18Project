CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm

compilerTest: main_test.c scanner_test.c
	$(CC) $(CFLAGS) -o $@ main_test.c scanner_test.c

clean:
	rm -f *.o compilerTest
