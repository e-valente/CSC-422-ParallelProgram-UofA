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
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include "utils.h"



#ifndef DEBUG
#define DEBUG 0
#endif

//mid contains the last vector's right session 
void merge(int left, int mid, int right)
{
    int i, j;
    
    i = left;
    j = mid;
    int k = 0;
   
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



int main(int argc, char **argv) {
  
  char *filename;
  int array_length;
  struct timeval startTime, endTime;
  struct responsetime response_time;
  pid_t kidpid, returnpid;
  int kid_status;
  
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
  
  merged_array = mmap(NULL, sizeof(char) * (MAXLINES * CHARS_PER_LINE), PROT_READ | PROT_WRITE, 
       MAP_ANONYMOUS | MAP_SHARED, -1, 0);
  
  
  if(merged_array == MAP_FAILED) {
    fprintf(stderr, "Error mapping memory!\n");
    exit(1);
  }
  
  array_length = createArrayfromFile(filename);

  
  
  gettimeofday(&startTime, NULL);
  
  kidpid = fork();
  //child
  if(kidpid == 0) {
   // printf("child\n");
    quick_sort(array_length/2 +1, array_length -1);
    return 0;
    
  } 
  //parent
  else {
     //printf("parent\n");
    quick_sort(0, array_length/2);
    
  }
  
  returnpid = waitpid(kidpid, &kid_status, 0);
  merge(0, array_length/2 +1, (array_length ));
  
  gettimeofday(&endTime, NULL);
  
  
  calculateDeltaTime(startTime, endTime, &response_time);
  
 
  //print the content case debug is set
  if(!DEBUG) {
  fprintf(stdout, "total: %d\n", array_length);
  for(int i =0; i < array_length/2 +1; i++)
    fprintf(stdout,"%s", (char*)merged_array +  (i) * SIZE_OF_LINE);
  
  fprintf(stdout, "--------\n");
  for(int i =array_length/2 +1; i < array_length; i++)
    fprintf(stdout,"%s", (char*)merged_array +  (i) * SIZE_OF_LINE);
  }
  
  
  printf("Result: %d seconds %0.3lf milliseconds\n", response_time.seconds, 
	 response_time.milliseconds);
      
  
  
  munmap(array, MAXLINES * SIZE_OF_LINE);
  munmap(merged_array, MAXLINES * SIZE_OF_LINE);
  
  

  return 0;
  
}