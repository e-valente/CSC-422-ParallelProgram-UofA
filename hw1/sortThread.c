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

/* Each worker prints its identity then
 * exits without returning anything */
void
*Worker( void *arg ) {
   long myid = (long) arg;
   printf("worker %ld (pthread id %ld) has started\n", myid, pthread_self());
int i;
for (i = 0; i < 1000000; i++)
  ;

   //usleep(HALF_SECOND);
   printf("worker %ld (pthread id %ld) is stopping\n", myid, pthread_self());
   return NULL;
} /* Worker */

void *func(void *arg) {
  long myid = (long) arg;

  
   printf("func %ld (pthread id %ld) has started\n", myid, pthread_self());
  
  return NULL;
}

void *executeQuickSortUsingThread(void *arg){ 
   long i = (long)arg;
   
   quick_sort(i * (array_length/total_processes), (i+1) * (array_length/total_processes) -1);
   pthread_exit(NULL);
}
  
  
void *executeMergeUsingThread(void *arg) {
  
  struct thread_args *myargs = (struct thread_args*)arg;
  
   int total_processes_to_merge = myargs->arg1;
   //printf("total_processes %d\n", total_processes_to_merge);
   double delta_size = array_length/total_processes_to_merge;
   
   long j = myargs->arg2;
  //printf("delta dize %lf j = %ld\n", delta_size, j);
  int start = j * delta_size;
  int end = start + delta_size;
	
  //void merge(int left, int mid, int right, int merge_direction)
 // printf("merge: %d %d %d %d\n", start, (start + end)/2, end, merge_direction);
  merge(start, (start + end)/2, end, merge_direction);
  
  pthread_exit(NULL);
  
}  
/* read command line, initialize, and
 * create threads */
int
main( int argc, char *argv[] ) {
   long i, total_merge_iterations;
   int numThreads;
   char *filename;
   struct timeval startTime, endTime;
   struct responsetime response_time;
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
  
  //EXECUTES QUICK_SORT()
  gettimeofday(&startTime, NULL);
  for(i = 0; i < total_processes; i++) {
      long j = i;
      pthread_create( &threadid[i], &attr, executeQuickSortUsingThread, (void *) j);
  }
  //WAITS
  for(int j = 0; j < total_processes; j++) 
     pthread_join(threadid[j], NULL);
     
  
   
  //MERGE_PROCESS
  merge_direction = FALSE;
  
   total_merge_iterations = exponentOfPowerOf2(total_processes);
  //the last merge is out of the for loop
  total_merge_iterations--; 
  

  
  
   struct thread_args mythreadargs[MAXPROCESSES];
  //if > 1 we have 2 or more processes
  if(total_merge_iterations >0 ) {
    
	
    for(int i=0; i<total_merge_iterations; i++) {
      
      //for 4 process total_merge_iterations = 1
      int total_processes_to_merge = (int)pow(2, total_merge_iterations -i);
     // double delta_size = array_length/total_processes_to_merge;
      
      //mythreadargs.arg1 = total_processes_to_merge;
    
      for(int j = 0; j < total_processes_to_merge; j++) { 
	 
	  mythreadargs[j].arg1 = total_processes_to_merge;
	  mythreadargs[j].arg2 = j;
	  pthread_create(&threadid[j], &attr, executeMergeUsingThread, (void *) &mythreadargs[j]);
      }
    
	
    //WAIT all processes to merge
      for(int j=0; j < total_processes_to_merge; j++)
	 pthread_join(threadid[j], NULL);
      
       change_merge_direction();
     
 
    }//for    
    
  }
  
  if(total_merge_iterations == 0 && total_processes > 2)
    change_merge_direction();
  
  merge(0, array_length/2, (array_length), merge_direction);
  gettimeofday(&endTime, NULL);
  change_merge_direction();
  
  calculateDeltaTime(startTime, endTime, &response_time);
  
   
   //change_merge_direction();
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
  //pthread_exit( NULL );
  // pthread_exit(0);
  

} /* main */