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
#include <math.h>
#include "utils.h"



#ifndef DEBUG
#define DEBUG 0
#endif

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


int main(int argc, char **argv) {
  
  char *filename;
  int total_merge_iterations;
  struct timeval startTime, endTime;
  struct responsetime response_time;
  pid_t kidpid[MAXPROCESSES], returnpid;
  int kid_status;
  
  if(argc < 3) {
    fprintf(stderr, "Usage: %s <# processes> <input_file>\n\n", argv[0]);
    exit(1);
  }
  
  total_processes = atoi(argv[1]);
  if(isPowerOfTwo(total_processes) == FALSE || total_processes == 0 || 
    total_processes == 1) {
    fprintf(stderr,"The processes must be power of 2\n\n");
    exit(1);
  }
  
  filename = argv[2];
  
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
  
  for(int i = 0; i < total_processes; i++) {
   kidpid[i] = fork();
   
  //child
  if(kidpid[i] == 0) {
    quick_sort(i * (array_length/total_processes), (i+1) * (array_length/total_processes) -1);
    return 0; 
    } 
  }//for 
  
  
  //WAIT QUICKSORT
  for(int i=0; i < total_processes; i++)
    returnpid = waitpid(kidpid[i], &kid_status, 0);
  
  
  total_merge_iterations = exponentOfPowerOf2(total_processes);
  //the last merge is out of the for loop
  total_merge_iterations--; 
  
  merge_direction = FALSE;
  
  //if > 1 we have 2 or more processes
  if(total_merge_iterations >0 ) {
    
	
    for(int i=0; i<total_merge_iterations; i++) {
      
      //for 4 process total_merge_iterations = 1
      int total_processes_to_merge = (int)pow(2, total_merge_iterations -i);
      double delta_size = array_length/total_processes_to_merge;
    
      for(int j = 0; j < total_processes_to_merge; j++) {
	kidpid[j] = fork();
	//child
	if(kidpid[j] == 0) {
    

	int start = j * delta_size;
	int end = start + delta_size;
	
	//void merge(int left, int mid, int right, int merge_direction)
	merge(start, (start + end)/2, end, merge_direction);
	return 0;
	}
	
      }
	
	 //WAIT all processes to merge
      for(int j=0; j < total_processes_to_merge; j++)
	returnpid = waitpid(kidpid[j], &kid_status, 0);
      
       change_merge_direction();
     
 
    }//for    
    
  }
  
  printf("total_merge_iterations %d\n", total_merge_iterations);
  
  if(total_merge_iterations == 0 && total_processes > 2)
    change_merge_direction();
  
  merge(0, array_length/2, (array_length), merge_direction);
  gettimeofday(&endTime, NULL);
  change_merge_direction();
  

  
  calculateDeltaTime(startTime, endTime, &response_time);
  
 
  //print the content case debug is set
  
  for(int i =0; i < array_length; i++) {
    if(merge_direction == TRUE)
      fprintf(stdout,"%s", (char*)merged_array +  (i) * SIZE_OF_LINE);
    else
      fprintf(stdout,"%s", (char*)array +  (i) * SIZE_OF_LINE);
    }
  
  
  
  printf("Result: %d seconds %0.3lf milliseconds\n", response_time.seconds, 
	 response_time.milliseconds);
      
  
  
  munmap(array, MAXLINES * SIZE_OF_LINE);
  munmap(merged_array, MAXLINES * SIZE_OF_LINE);
  
  

  return 0;
  
}