/*Example of how to use UNIX (POSIX)
 * to create a new process
 * Copyright (C) 2015 emanuelvalente@gmail.com
 * 
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
  
  pid_t kidpid, returnpid;
  int i, kid_status;
  

  
    kidpid = fork();
  
    if(kidpid < 0) {
      fprintf(stderr, "fork() has failed!\n");
      exit(1);
    }
  
    if(kidpid == 0) {
      printf("-----------> Hi, I am the child, and my pid is %d\n", (int)getpid());
      sleep(2);
      
    }
    else 
      printf("-->I am the parent, my pid is %d and my child is %d\n", (int)getpid(), (int)kidpid);
  
  
  /*both process will execute this statement*/
  printf("Last printf, pid is %d\n", (int)getpid());
  
  //returnpid will be -1 if the process is the child itself
  //kid_status can be read after child process terminates
  returnpid = waitpid(kidpid, &kid_status, 0);
  printf("Last printf2, pid is %d\nreturnpid %d\nkid_status = %d\n\n", (int)getpid(), returnpid, (int)kid_status);
    
   if (WIFEXITED(kid_status))
      printf("Parent: Child %d exit; kid_status = %d\n",
             (int)returnpid, WEXITSTATUS(kid_status));
   else
      printf("Parent: Child %d did not use exit\n", (int)returnpid);


   return 0;
}