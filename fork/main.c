/*Example of how to use UNIX (POSIX)
 * to create a new process
 * Copyright (C) 2015 emanuelvalente@gmail.com
 * 
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  
  pid_t kidpid;
  
  kidpid = fork();
  
  if(kidpid < 0) {
    fprintf(stderr, "fork() has failed!\n");
    exit(1);
  }
  
  if(kidpid == 0) 
    printf("Hi, I am the kid, and my pid is %d\n", (int)getpid());
  else printf("I am the parent, my pid is %d\n", (int)getpid());
  
  /*both process will execute this statement*/
  printf("Last printf, pid is %d\n", (int)getpid());
  
  
  return 0;
  
  
}
