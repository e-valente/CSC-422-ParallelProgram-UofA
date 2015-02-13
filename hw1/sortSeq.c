/*CSC422 - HW1
 * Copyright (C) 2015 emanuelvalente@gmail.com
 * gcc sortSeq.c -o test -std=c99 or
 * gcc sortSeq.c -o test -std=gnu99 -DDEBUG=1 

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

int medianOfThree(int left, int right) {
  int mid;
  
  mid = (left + right) / 2;
  
  if( (array + left * SIZE_OF_LINE) > (array + mid * SIZE_OF_LINE ) ) {
   if(left != mid)
    swap(left, mid);
  }
  
  if((array + left * SIZE_OF_LINE) > (array + right * SIZE_OF_LINE)) {
    if(left != right)
      swap(left, right);
  }
  
  if((array + mid * SIZE_OF_LINE) > (array + right * SIZE_OF_LINE)) {
    if(mid != right)
      swap(mid, right);
  }
  
  swap(mid, right -1);
  
  return right -1;

  
}
  
int partitionMedianOfThree(int leftIndex, int rightIndex, int pivotIndex) {
  
  int left = leftIndex;
  int right = rightIndex;
  
  while(1) {
    
    while( (array + (++left * SIZE_OF_LINE)) < (array + pivotIndex * SIZE_OF_LINE));
    
    while((array + (--right * SIZE_OF_LINE)) > (array + pivotIndex * SIZE_OF_LINE));
    
    if(left >= right)
      break;
    else
      if(left != right)
	swap(left, right);
    
    
  }
  
  swap(left, rightIndex -1);  
  return left;  
   
}
/*
 * def MedianOfThree(arr, left, right):
    mid = (left + right)/2
    if arr[right] < arr[left]:
        Swap(arr, left, right)        
    if arr[mid] < arr[left]:
        Swap(arr, mid, left)
    if arr[right] < arr[mid]:
        Swap(arr, right, mid)
    return mid
    
    */

void quick_sort(int left, int right) {
  int r;
  
  if(right > left) {
    r = partition(left, right);
  
    quick_sort(left, r-1);
    quick_sort(r+1, right);    
  }
  
  
}

void quick_sort2(int left, int right) {
  int mid, r;
  
  if(right > left) {
    mid = medianOfThree(left, right);
    r = partitionMedianOfThree(left, right, mid);
    quick_sort2(left, r-1);
    quick_sort2(r+1, right);    
  }
  
  
}
  

int main(int argc, char **argv) {
  
  char *filename;
  int array_length;
  struct timeval startTime, endTime;
  struct responsetime response_time;
  
  if(argc < 2) {
    fprintf(stderr, "Usage: %s <input_file>\n\n", argv[0]);
    exit(1);
  }
  
  filename = argv[1];
  
   array = mmap(NULL, sizeof(char) * (MAXLINES * CHARS_PER_LINE), PROT_READ | PROT_WRITE, 
       MAP_ANONYMOUS | MAP_SHARED, -1, 0);
  
  if(array == MAP_FAILED) {
    fprintf(stderr, "Error mapping memory!\n");
    exit(1);
  }
  
  array_length = createArrayfromFile(filename);
  
 
  
  gettimeofday(&startTime, NULL);
  quick_sort(0, array_length -1);
  gettimeofday(&endTime, NULL);
  
  calculateDeltaTime(startTime, endTime, &response_time);
  
  
  
  
  //print the content case debug is set
  if(DEBUG) {
  for(int i =0; i < array_length; i++)
    fprintf(stdout,"%s", (char*)array +  (i) * SIZE_OF_LINE);
  }
  
  printf("Result: %d seconds %0.3lf milliseconds\n", response_time.seconds, 
	 response_time.milliseconds);
      
  
  
  munmap(array, MAXLINES * SIZE_OF_LINE);
  
  return 0;
  
}