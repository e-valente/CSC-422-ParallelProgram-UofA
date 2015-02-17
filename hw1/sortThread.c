/* simple program illustrating passing 
 * an argument to a thread.
   gcc -Wall -std=gnu99 -o simple simple.c -lpthread
   simple numWorkers
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "utils.h"


/*function that threads executes for quicksort process*/
void *executeQuickSortUsingThread(void *arg){ 
   long i = (long)arg;
   
   quick_sort(i * (array_length/total_processes), (i+1) * (array_length/total_processes) -1);
   pthread_exit(NULL);
}
  

/*function that threads executes for merging process*/
void *executeMergeUsingThread(void *arg) {
  
  struct thread_args *myargs = (struct thread_args*)arg;
  
  int total_processes_to_merge = myargs->arg1;
  /*chunk for each array section*/
  double delta_size = array_length/total_processes_to_merge; 
  long j = myargs->arg2;
  
  int start = j * delta_size;
  int end = start + delta_size;
	
 // printf("merge: %d %d %d %d\n", start, (start + end)/2, end, merge_direction);
  merge(start, (start + end)/2, end, merge_direction);
  
  pthread_exit(NULL);
  
}  


int main( int argc, char *argv[] ) {
   long i, total_merge_iterations;
   int numThreads;
   char *filename;
   struct timeval startTime, endTime, responseTime;
   pthread_attr_t attr;
   pthread_t threadid[MAXPROCESSES];

   
    if(argc < 3) {
    fprintf(stderr, "Usage: %s <# threads> <input_file>\n\n", argv[0]);
    exit(1);
  }

   /* set global thread attributes */
   pthread_attr_init( &attr );
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
   pthread_attr_setscope( &attr, PTHREAD_SCOPE_SYSTEM );

   /* read command line, no error
    * checking done here */
   numThreads = atoi( argv[1] );
   total_processes = numThreads;
   
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
  
  /*starting count the time: quicksort(s) threads*/
  gettimeofday(&startTime, NULL);
  for(i = 0; i < total_processes; i++) {
      long j = i;
      pthread_create( &threadid[i], &attr, executeQuickSortUsingThread, (void *) j);
  }
  
  /*waits for all quicksort process*/
  for(int j = 0; j < total_processes; j++) 
     pthread_join(threadid[j], NULL);
     
  
   
  /*initiates the merging direction.
   * FALSE: array->merged_array
   * TRUE: merged_array -> array
   */
  merge_direction = FALSE;
  
  /*how many iterations for the merging process?*/
  total_merge_iterations = exponentOfPowerOf2(total_processes);
  //the last merge is out of the for loop
  total_merge_iterations--; 
  
  /*this struct stores our args that we send to threads*/
  struct thread_args mythreadargs[MAXPROCESSES];
  //if > 1 we have 2 or more processes
  if(total_merge_iterations >0 ) {
    
	
    for(int i=0; i<total_merge_iterations; i++) {
      
      //for example: for 4 threads total_merge_iterations = 1
      int total_processes_to_merge = (int)pow(2, total_merge_iterations -i);
      
      for(int j = 0; j < total_processes_to_merge; j++) {
	  /*setting our args*/
	  mythreadargs[j].arg1 = total_processes_to_merge;
	  mythreadargs[j].arg2 = j;
	  pthread_create(&threadid[j], &attr, executeMergeUsingThread, (void *) &mythreadargs[j]);
      }
    
	
      //waits all threads for merging
      for(int j=0; j < total_processes_to_merge; j++)
	 pthread_join(threadid[j], NULL);
      
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

} /* main */