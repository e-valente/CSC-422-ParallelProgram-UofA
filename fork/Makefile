CC=gcc
CFLAGS=-Wall -std=gnu99

debug:clean
	$(CC) $(CFLAGS) -g -o fork main.c
stable:clean
	$(CC) $(CFLAGS) -o fork main.c
	$(CC) $(CFLAGS) -o fork2 fork2.c
clean:
	rm -vfr *~ fork fork2
