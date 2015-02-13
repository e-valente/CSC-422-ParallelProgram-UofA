#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"



int createArrayfromFile(char *filename, char ***array_)
{
  char **array;
  FILE *myfile;
  char *filecontent;
  char *ret;
  int length, count;

  
  myfile = fopen(filename, "r");
  
  fseek(myfile, 0, SEEK_END);
  length = ftell(myfile);
  fseek(myfile,0, SEEK_SET);
  
  //contais a one dimension array with all the content of file
  filecontent = (char*)malloc(sizeof(char) * (length +1));
  
  count = 0;
  
  //reads the file content
  int spaces = 0;
  int flagspace = 0;
  while((filecontent[count] = fgetc(myfile)) != EOF) {
    if(filecontent[count] == ' ' || filecontent[count] == '\n') {
      if(flagspace == 0) {
	spaces++;
      }
      
      flagspace = 1;
    }
    else flagspace = 0;
    
    count++;
  }
  filecontent[count-1] = '\0';
  
  //find the substrings 
  ret = strtok(filecontent, " \n");
  count = 0;
  
  array = (char**)malloc(sizeof(char*) * spaces);
  
  do {
    array[count] = (char*)malloc(sizeof(char) * (strlen(ret) + 1)); 
      strcpy(array[count++], ret);
      //printf("%s\n", ret);
   
  }while((ret = strtok(NULL, " \n")) != NULL);
  
  *array_ = array;
  
 
  free(filecontent);
  fclose(myfile);
  
  
  //corrects bufs for some files
  if(array[spaces -1] == NULL) spaces--;
  
  //spaces is actually the length of the array
  return spaces;
  
}

void calculateDeltaTime(struct timeval start, struct timeval end, struct responsetime *res) {
  double milliseconds;
  int seconds;
  int usecs;
  
  seconds = end.tv_sec - start.tv_sec;
  usecs = end.tv_usec - start.tv_usec;
  
  if(end.tv_usec < start.tv_usec) {
    seconds--;
    usecs += 1000000;
  }
  
  milliseconds = 0.00000;
  milliseconds += usecs * 0.001;
  
  res->seconds = seconds;
  res->milliseconds = milliseconds;
}
