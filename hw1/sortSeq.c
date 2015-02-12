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


#ifndef DEBUG
#define DEBUG 0
#endif

struct responsetime {
  int seconds;
  double milliseconds;
};


int createArrayfromFile(char *filename, char ***array_)
{
  char **array;
  FILE *myfile;
  char *filecontent;
  char *ret;
  int length, count;

  
  myfile = fopen(filename, "r");
  
  fseek(myfile, 0, SEEK_END);
  length = ftell(myfile);
  fseek(myfile,0, SEEK_SET);
  
  //contais a one dimension array with all the content of file
  filecontent = (char*)malloc(sizeof(char) * (length +1));
  
  count = 0;
  
  //reads the file content
  int spaces = 0;
  int flagspace = 0;
  while((filecontent[count] = fgetc(myfile)) != EOF) {
    if(filecontent[count] == ' ' || filecontent[count] == '\n') {
      if(flagspace == 0) {
	spaces++;
      }
      
      flagspace = 1;
    }
    else flagspace = 0;
    
    count++;
  }
  filecontent[count-1] = '\0';
  
  //find the substrings 
  ret = strtok(filecontent, " \n");
  count = 0;
  
  array = (char**)malloc(sizeof(char*) * spaces);
  
  do {
    array[count] = (char*)malloc(sizeof(char) * (strlen(ret) + 1)); 
      strcpy(array[count++], ret);
      //printf("%s\n", ret);
   
  }while((ret = strtok(NULL, " \n")) != NULL);
  
  *array_ = array;
  
 
  free(filecontent);
  fclose(myfile);
  
  
  //corrects bufs for some files
  if(array[spaces -1] == NULL) spaces--;
  
  //spaces is actually the length of the array
  return spaces;
  
}

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


int main(int argc, char **argv) {
  
  char *filename;
  char **array;
  int array_length;
  struct timeval startTime, endTime;
  struct responsetime response_time;
  
  if(argc < 2) {
    fprintf(stderr, "Usage: %s <input_file>\n\n", argv[0]);
    exit(1);
  }
  
  filename = argv[1]; 
  //creates the array
  array_length = createArrayfromFile(filename, &array);
  
  gettimeofday(&startTime, NULL);
  quick_sort2(array, 0, array_length -1);
  gettimeofday(&endTime, NULL);
  
  calculateDeltaTime(startTime, endTime, &response_time);
  
  printf("Result: %d seconds %0.3lf milliseconds\n", response_time.seconds, 
	 response_time.milliseconds);
  
  
  //print the content case debug is set
  if(DEBUG) {
  for(int i =0; i < array_length; i++)
    printf("%s \n", array[i]);
      
  }
  
  //FREE memory from array
  for(int i =0; i < array_length; i++)
    free(array[i]);
  free(array);

  return 0;
  
}