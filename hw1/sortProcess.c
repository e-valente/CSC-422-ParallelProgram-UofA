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

int main(int argc, char **argv) {
  
  char *filename;
  int total_merge_iterations;
  struct timeval startTime, endTime, responseTime;
  pid_t kidpid[MAXPROCESSES];
  int kid_status;
  
  if(argc < 3) {
    fprintf(stderr, "Usage: %s <# processes> <input_file>\n\n", argv[0]);
    exit(1);
  }
  
  total_processes = atoi(argv[1]);
  /*checks the input: must be powe of 2*/
  if(isPowerOfTwo(total_processes) == FALSE || total_processes == 0 || 
    total_processes == 1) {
    fprintf(stderr,"The processes must be power of 2\n\n");
    exit(1);
  }
  
  filename = argv[2];
  
  /*alloc our array and the auxiliar array (=merged_array) to the merging process*/
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

  /*starting count the time: quicksort(s) processes*/
  gettimeofday(&startTime, NULL);
  
  for(int i = 0; i < total_processes; i++) {
   kidpid[i] = fork(); 
  //child: at this point we do not worry with the parent process
  if(kidpid[i] == 0) {
    quick_sort(i * (array_length/total_processes), (i+1) * (array_length/total_processes) -1);
    return 0; 
    } 
  }
  
  /*waits for all quicksort process*/
  for(int i=0; i < total_processes; i++)
    waitpid(kidpid[i], &kid_status, 0);
  
  /*how many iterations for the merging process?*/
  total_merge_iterations = exponentOfPowerOf2(total_processes);
  //the last merge is out of the loop so we must decrement one
  total_merge_iterations--; 
  
  /*initiates the merging direction.
   * FALSE: array->merged_array
   * TRUE: merged_array -> array
   */
  merge_direction = FALSE;
  
  //if > 1 we have 2 or more processes
  if(total_merge_iterations >0 ) {
    	
    for(int i=0; i<total_merge_iterations; i++) {
      
      //for 4 process total_merge_iterations = 1
      int total_processes_to_merge = (int)pow(2, total_merge_iterations -i);
      /*chunk for each array section*/
      double delta_size = array_length/total_processes_to_merge;
    
      /*loops that controls how many process to each merging iteration*/
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
	
      //waits all processes to merge
      for(int j=0; j < total_processes_to_merge; j++)
	waitpid(kidpid[j], &kid_status, 0);
      
      /*changes the merging direction.
      * FALSE: array->merged_array
      * TRUE: merged_array -> array
      */
       change_merge_direction();    
 
    }//for    
    
  }
 
  /*corrects the direction of merging*/
  if(total_merge_iterations == 0 && total_processes > 2)
    change_merge_direction();
  
  /*last merge: at this point we have two sorted array subsessions*/
  merge(0, array_length/2, (array_length), merge_direction);
  gettimeofday(&endTime, NULL);
  change_merge_direction();
  
  /*calculates the time spent*/
  calculateDeltaTime(startTime, endTime, &responseTime);
  
  /*prints the sorted array*/
  for(int i =0; i < array_length; i++) {
    if(merge_direction == TRUE)
      fprintf(stdout,"%s", (char*)merged_array +  (i) * SIZE_OF_LINE);
    else
      fprintf(stdout,"%s", (char*)array +  (i) * SIZE_OF_LINE);
    }
   
  /*prints to stderr our result*/
  fprintf(stderr, "runtime: %ld seconds, %ld microseconds\n", responseTime.tv_sec, responseTime.tv_usec);

  /*free our memory*/
  munmap(array, MAXLINES * SIZE_OF_LINE);
  munmap(merged_array, MAXLINES * SIZE_OF_LINE);
  
  return 0;
  
}