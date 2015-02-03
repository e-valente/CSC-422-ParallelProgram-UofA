CC=gcc
CFLAGS=-Wall -std=gnu99

debug:clean
	$(CC) $(CFLAGS) -g -o fork main.c
stable:clean
	$(CC) $(CFLAGS) -o fork main.c
	$(CC) $(CFLAGS) -o fork2 fork2.c
	$(CC) $(CFLAGS) -o fork3-wait fork3-wait.c
clean:
	rm -vfr *~ fork fork2 fork3-wait
