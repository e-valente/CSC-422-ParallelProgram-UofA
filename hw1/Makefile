CC=gcc
CFLAGS=-Wall -std=gnu99
CFLAGS-DEBUG=-Wall -std=gnu99 -g -DDEBUG=1

all: sortSeq

sortSeq: sortSeq.o utils.o
	$(CC) $(CFLAGS) -o sortSeq sortSeq.o utils.o
sortSeq.o: sortSeq.c
	$(CC) $(CFLAGS) -c sortSeq.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c
debug:
	$(CC) $(CFLAGS-DEBUG) sortSeq.c -o sortSeq

clean:
	rm *.o sortSeq
