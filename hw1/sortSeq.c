/*CSC422 - HW1
 * Copyright (C) 2015 emanuelvalente@gmail.com
 * gcc sortSeq.c utils.c -o test -std=c99 or
 * gcc sortSeq.c utils.c -o test -std=gnu99 -DDEBUG=1 

 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/mman.h>
#include "utils.h"

#ifndef DEBUG
#define DEBUG 0
#endif

int main(int argc, char **argv) {
  
  char *filename;
  struct timeval startTime, endTime, responseTime;
  
  if(argc < 2) {
    fprintf(stderr, "Usage: %s <input_file>\n\n", argv[0]);
    exit(1);
  }
  
  filename = argv[1];
  
  //alloc our array
  array = mmap(NULL, sizeof(char) * (MAXLINES * CHARS_PER_LINE), PROT_READ | PROT_WRITE, 
       MAP_ANONYMOUS | MAP_SHARED, -1, 0);
  
  if(array == MAP_FAILED) {
    fprintf(stderr, "Error mapping memory!\n");
    exit(1);
  }
  
  array_length = createArrayfromFile(filename);
  
  /*call quick sort and calculates the time*/
  gettimeofday(&startTime, NULL);
  quick_sort(0, array_length -1);
  gettimeofday(&endTime, NULL);
  calculateDeltaTime(startTime, endTime, &responseTime);
  
  /*prints the array*/
  for(int i =0; i < array_length; i++)
    fprintf(stdout,"%s", (char*)array +  (i) * SIZE_OF_LINE);
  
  /*outputs the time spent*/
  fprintf(stderr, "runtime: %ld seconds, %ld microseconds\n", responseTime.tv_sec, responseTime.tv_usec);

  /*free our memory*/
  munmap(array, MAXLINES * SIZE_OF_LINE);
  
  return 0;
  
}