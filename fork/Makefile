CC=gcc
CFLAGS=-Wall -std=gnu99

debug:clean
	$(CC) $(CFLAGS) -g -o fork main.c
stable:clean
	$(CC) $(CFLAGS) -o fork main.c
clean:
	rm -vfr *~ fork
