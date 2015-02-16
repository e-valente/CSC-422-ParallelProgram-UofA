CC=gcc
#CFLAGS=-Wall -std=gnu99 -O2 -march=native -lm
CFLAGS=-Wall -std=gnu99 -lm
CFLAGS-DEBUG=-Wall -std=gnu99 -g -DDEBUG=1

all: sortSeq sortProcess sortThread

sortSeq: sortSeq.o utils.o
	$(CC) $(CFLAGS) -o sortSeq sortSeq.o utils.o

sortSeq.o: sortSeq.c utils.h
	$(CC) $(CFLAGS) -c sortSeq.c
	
sortProcess: sortProcess.o utils.o
	$(CC) $(CFLAGS) -o sortProcess sortProcess.o utils.o
	
sortProcess.o: sortProcess.c utils.h
	$(CC) $(CFLAGS) -c sortProcess.c	

sortThread: sortThread.o utils.o
	$(CC) $(CFLAGS) -o sortThread sortThread.o utils.o -lpthread

sortThread.o: sortThread.c utils.c utils.h
	$(CC) $(CFLAGS) -c sortThread.c utils.c -lpthread


utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c
debug:
	$(CC) $(CFLAGS-DEBUG) sortSeq.c utils.c -o sortSeq
	$(CC) $(CFLAGS-DEBUG) sortProcess.c utils.c -o sortProcess

clean:
	rm *.o sortSeq sortProcess sortThread
