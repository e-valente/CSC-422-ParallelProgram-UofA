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
    fprintf(stderr, "Error: Opening file %s\n\n", filename);
    exit(1);
    
  }
  count = 0;
  while(fgets(array + (count++ * SIZE_OF_LINE), SIZE_OF_LINE, myfile));
  
  
  fclose(myfile);
  return count -1;
  
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

void swap(int i, int j) {
  char tmp[SIZE_OF_LINE];
  
  strcpy(tmp, (array + i * SIZE_OF_LINE));
  
  strcpy((array + i * SIZE_OF_LINE), (array + j * SIZE_OF_LINE));
  strcpy((array + j * SIZE_OF_LINE), tmp); 
  
  
}

int partition(int left, int right) {
  int i, j;
  
  i = left;
  for(j = left + 1; j <= right; ++j) {
   // if(array[j] != NULL && array[left] != NULL) {
    if(strcmp((array + j * SIZE_OF_LINE), (array + left * SIZE_OF_LINE)) < 0) {
      ++i;
      if(i != j)
	swap(i, j);
    //}
    }
  }
  
  swap(left, i);
  
  return i;
  
}

void quick_sort(int left, int right) {
  int r;
  
  if(right > left) {
    r = partition(left, right);
  
    quick_sort(left, r-1);
    quick_sort(r+1, right);    
  }
  
  
}

