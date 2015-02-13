#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"


int createArrayfromFile(char *filename)
{
  
  FILE *myfile;
  int count;
  
  myfile = fopen(filename, "r");  
  
   if(myfile == NULL) {
    fprintf(stderr, "Error: Opening File!\n\n", filename);
    exit(1);
    
  }
  count = 0;
  while(fgets(array + (count++ * SIZE_OF_LINE), SIZE_OF_LINE, myfile));
  
  
  fclose(myfile);
  return count;
  
}

void calculateDeltaTime(struct timeval start, struct timeval end, struct responsetime *res) {
  double milliseconds;
  int seconds;
  int usecs;
  
  seconds = end.tv_sec - start.tv_sec;
  usecs = end.tv_usec - start.tv_usec;
  
  if(end.tv_usec < start.tv_usec) {
    seconds--;
    usecs += 1000000;
  }
  
  milliseconds = 0.00000;
  milliseconds += usecs * 0.001;
  
  res->seconds = seconds;
  res->milliseconds = milliseconds;
}
