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
  
  pid_t kidpid[3], returnpid;
  int i, kid_status;
  
  
  for(i = 0; i < 3; i++) {
    
    kidpid[i] = fork();
  
    if(kidpid[i] < 0) {
      fprintf(stderr, "fork() has failed!\n");
      exit(1);
    }
  
    if(kidpid[i] == 0) {
      printf("--> Hi, I am the child, and my pid is %d\n", (int)getpid());
      sleep(5);
      
    }
    else 
      printf("I am the parent, my pid is %d and my child is %d\n", (int)getpid(), (int)kidpid[i]);
  }
  
  /*both process will execute this statement*/
  printf("Last printf, pid is %d\n", (int)getpid());
  
  returnpid = waitpid(kidpid[2], &kid_status, 0);

   if (WIFEXITED(kid_status))
      printf("Parent: Child %d exit; kid_status = %d\n",
             (int)returnpid, WEXITSTATUS(kid_status));
   else
      printf("Parent: Child %d did not use exit\n", (int)returnpid);

   for (i = 0; i < 2; i++) {

      returnpid = waitpid(-1, &kid_status, 0);

      if (WIFEXITED(kid_status))
         printf("Parent: Child %d exit; kid_status = %d\n",
                (int)returnpid, WEXITSTATUS(kid_status));
      else
         printf("Parent: Child %d did not use exit\n", (int)returnpid);
   }

   printf("Last printf, pid = %d\n", (int)getpid());

   return 0;
} /* main */
   
