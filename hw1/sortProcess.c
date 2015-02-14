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
void merge(int left, int mid, int right, int direction)
{
    int i, j;
    
    i = left;
    j = mid;
    int k = left;
    
    
   //array -> merged_array
    if(direction == 0) {
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



int main(int argc, char **argv) {
  
  char *filename;
  int array_length;
  struct timeval startTime, endTime;
  struct responsetime response_time;
  pid_t kidpid[4], kidpid2[4], returnpid;
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
  
  for(int i = 0; i < 4; i++) {
  
   kidpid[i] = fork();
  //child
  if(kidpid[i] == 0) {
      //printf("--> Hi, I am the child %d, and my pid is %d, my parent id is %d\n", i, (int)getpid(), getppid());

    //quick_sort(array_length/2 +1, array_length -1);
    // printf("--> QUICK %d %d\n", i * (array_length/4), (i+1) * (array_length/4) -1);
     quick_sort(i * (array_length/4), (i+1) * (array_length/4) -1);
    return 0;
    
    } 
 
  }//for 
  
  //WAIT QUICKSORT
  for(int i=0; i < 4; i++)
    returnpid = waitpid(kidpid[i], &kid_status, 0);
  
  
  //MERGE
  for(int i = 0; i < 2; i++) {
  
   kidpid2[i] = fork();
  //child
  if(kidpid2[i] == 0) {
      //printf("--> Hi, I am the child %d, and my pid is %d, my parent id is %d\n", i, (int)getpid(), getppid());
      int start = i * (array_length/2);
      int end = (i+1) * (array_length/2);

    //quick_sort(array_length/2 +1, array_length -1);
    //merge(0, array_length/2 +1, (array_length ));
      //printf("merge %d %d %d\n\n",start, start + (end - start)/2 + 1, end);
      merge(start, start + (end - start)/2, end, 0);
    return 0;
    
  } 
 
 
  }//for 
  
  returnpid = waitpid(kidpid2[0], &kid_status, 0);
  returnpid = waitpid(kidpid2[1], &kid_status, 0);
 // memcpy((char*)array,(char*) merged_array, sizeof(char) * (MAXLINES * CHARS_PER_LINE));  
  merge(0, array_length/2, (array_length), 1);
  
  gettimeofday(&endTime, NULL);
  
  
  calculateDeltaTime(startTime, endTime, &response_time);
  
 
  //print the content case debug is set
  if(!DEBUG) {
  fprintf(stdout, "total: %d\n", array_length);
  for(int i =0; i < array_length; i++)
    //fprintf(stdout,"%s", (char*)merged_array +  (i) * SIZE_OF_LINE);
    fprintf(stdout,"%s", (char*)array +  (i) * SIZE_OF_LINE);
  /*
  fprintf(stdout, "--------\n");
  for(int i =array_length/2; i < array_length; i++)
    fprintf(stdout,"%s", (char*)merged_array +  (i) * SIZE_OF_LINE);
    //fprintf(stdout,"%s", (char*)array +  (i) * SIZE_OF_LINE);
  */
  }
  
  
  printf("Result: %d seconds %0.3lf milliseconds\n", response_time.seconds, 
	 response_time.milliseconds);
      
  
  
  munmap(array, MAXLINES * SIZE_OF_LINE);
  munmap(merged_array, MAXLINES * SIZE_OF_LINE);
  
  

  return 0;
  
}