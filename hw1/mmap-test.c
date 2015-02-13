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


void swap(char **array, int i, int j) {
  char *tmp;
  int len_i = strlen(array[i]);
  int len_j = strlen(array[j]);
  
  tmp = (char*)malloc(sizeof(char)* (len_i + 1));
  strcpy(tmp, array[i]);
  
  if(len_i < len_j) 
    array[i] = (char*)realloc(array[i], sizeof(char) * (len_j +1));
  
  else if(len_i > len_j) 
    array[j] = (char*)realloc(array[j], sizeof(char) * (len_i +1));
  
  strcpy(array[i], array[j]);
  strcpy(array[j], tmp); 
  
  free(tmp);
  
  
}

int partition(char **array, int left, int right) {
  int i, j;
  
  i = left;
  for(j = left + 1; j <= right; ++j) {
   // if(array[j] != NULL && array[left] != NULL) {
    if(strcmp(array[j], array[left]) < 0) {
      ++i;
      swap(array, i, j);
    //}
    }
  }
  
  swap(array, left, i);
  
  return i;
  
}

int medianOfThree(char **array, int left, int right) {
  int mid;
  
  mid = (left + right) / 2;
  
  if(array[left] > array[mid]) 
    swap(array, left, mid);
  
  if(array[left] > array[right]) 
    swap(array, left, right);
  
  if(array[mid] > array[right]) 
    swap(array, mid, right);
  
  swap(array, mid, right -1);
  
  return right -1;

  
}
  
int partitionMedianOfThree(char **array, int leftIndex, int rightIndex, int pivotIndex) {
  
  int left = leftIndex;
  int right = rightIndex;
  
  while(1) {
    
    while(array[++left] < array[pivotIndex]);
    
    while(array[--right] > array[pivotIndex]);
    
    if(left >= right)
      break;
    else
      swap(array, left, right);
    
    swap(array, left, rightIndex -1);
    
    return left;  
  }
  
  return -1;
   
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

void quick_sort(char **array, int left, int right) {
  int r;
  
  if(right > left) {
    r = partition(array, left, right);
  
    quick_sort(array, left, r-1);
    quick_sort(array, r+1, right);    
  }
  
  
}

void quick_sort2(char **array, int left, int right) {
  int mid, r;
  
  if(right > left) {
    mid = medianOfThree(array, left, right);
    r = partitionMedianOfThree(array, left, right, mid);
    quick_sort(array, left, r-1);
    quick_sort(array, r+1, right);    
  }
  
  
}
  

int main(int argc, char **argv) {
  
  char *filename;
  char **array;
  int array_length;
  struct timeval startTime, endTime;
  struct responsetime response_time;
  char mystr[CHARS_PER_LINE];
  char mystr2[100];
  FILE *myfile;
  int count;
  
  void *buffer;
  
  if(argc < 2) {
    fprintf(stderr, "Usage: %s <input_file>\n\n", argv[0]);
    exit(1);
  }
  
  buffer = mmap(NULL, sizeof(char) * (MAXLINES * CHARS_PER_LINE), PROT_READ | PROT_WRITE, 
       MAP_ANONYMOUS | MAP_SHARED, -1, 0);
  
  if(buffer == MAP_FAILED) {
    fprintf(stderr, "Error mapping memory!\n");
    exit(1);
  }
  /*
  strcpy(mystr,"Agora vai!");
  strcpy(mystr2,"Agora vai2!");
  
  array = buffer;
  
  memcpy(buffer, mystr, SIZE_OF_LINE);
  memcpy(buffer + SIZE_OF_LINE , mystr2, SIZE_OF_LINE);
  
  fprintf(stdout, "%s\n", (char*)buffer + SIZE_OF_LINE); 
  */
  
  filename = argv[1];
  myfile = fopen(filename, "r");
  
  if(myfile == NULL) {
    fprintf(stderr, "Error: Opening File!\n\n", argv[0]);
    exit(1);
    
  }
  
  
  count = 0;
  
  //while(fgets(mystr, SIZE_OF_LINE, myfile)) {
  while(fgets(buffer + (count++ * SIZE_OF_LINE), SIZE_OF_LINE, myfile)) {
     //fprintf(stdout,"%s", mystr);
     //memcpy(buffer + count++ * SIZE_OF_LINE , mystr, SIZE_OF_LINE);
     fprintf(stdout,"%s", (char*)buffer +  (count-1) * SIZE_OF_LINE );
      //fgets(buffer + count++ * SIZE_OF_LINE, SIZE_OF_LINE, myfile);
  }
  
  
  
  
  
  fclose(myfile);
  
  
  munmap(buffer, MAXLINES * SIZE_OF_LINE);
  
 
  
 
  
  //FREE memory from array
 
  return 0;
  
}