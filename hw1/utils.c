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

void calculateDeltaTime(struct timeval start, struct timeval end, struct timeval *res) {
  int seconds;
  int usecs;
  
  seconds = end.tv_sec - start.tv_sec;
  usecs = end.tv_usec - start.tv_usec;
  
  if(end.tv_usec < start.tv_usec) {
    seconds--;
    usecs += 1000000;
  }
  
 
  res->tv_sec = seconds;
  res->tv_usec = usecs;
  
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

int isPowerOfTwo(int n){
  if (n == 0)
    return TRUE;
  while(n != 1) {
    if(n%2 != 0)
      return FALSE;
    n = n/2;
  }
  return TRUE;
}

int exponentOfPowerOf2(int n){
  
  int exponent = 0;
  
  if (n %2 != 0)
    return FALSE;
  
  while(n > 1) {
    n = n/2;
    exponent += 1;
  }
  return exponent;
}

void change_merge_direction() {
 if(merge_direction == FALSE) 
    merge_direction = TRUE;
 else
  merge_direction = FALSE;
}

//mid contains the last vector's right session 
//mid contains the last vector's right session 
void merge(int left, int mid, int right, int merge_direction)
{
    int i, j;
    
    i = left;
    j = mid;
    int k = left;
    
    
   //array -> merged_array
    if(merge_direction == FALSE) {
      while(i < mid && j < right) {
	if(strcmp((array + i * SIZE_OF_LINE), (array + j * SIZE_OF_LINE)) < 0) 
	    strcpy((merged_array + k++ * SIZE_OF_LINE), (array + i++ * SIZE_OF_LINE));
	
	else 
	  strcpy((merged_array + k++ * SIZE_OF_LINE), (array + j++ * SIZE_OF_LINE));   
	
      }
      
      while(i < mid) 
	strcpy((merged_array + k++ * SIZE_OF_LINE), (array + i++ * SIZE_OF_LINE));
      
      while(j < right) 
       strcpy((merged_array + k++ * SIZE_OF_LINE), (array + j++ * SIZE_OF_LINE));
    }
    
    //direction != 0
    //=> merged_array -> array
    else {
      while(i < mid && j < right) {
	if(strcmp((merged_array + i * SIZE_OF_LINE), (merged_array + j * SIZE_OF_LINE)) < 0) 
	    strcpy((array + k++ * SIZE_OF_LINE), (merged_array + i++ * SIZE_OF_LINE));
	
	else 
	  strcpy((array + k++ * SIZE_OF_LINE), (merged_array + j++ * SIZE_OF_LINE));   
	
      }
      
      while(i < mid) 
	strcpy((array + k++ * SIZE_OF_LINE), (merged_array + i++ * SIZE_OF_LINE));
      
      while(j < right) 
       strcpy((array + k++ * SIZE_OF_LINE), (merged_array + j++ * SIZE_OF_LINE));
      
      
    }
    

}  
