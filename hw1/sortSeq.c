/*CSC422 - HW1
 * Copyright (C) 2015 emanuelvalente@gmail.com
 *  gcc sortSeq.c -o test -std=c99
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  
  char *filename;
  char *filecontent;
  char *ret;
  char **array;
  char c;
  FILE *myfile;
  int length, count;
  
  if(argc < 2) {
    fprintf(stderr, "Usage: %s <input_file>\n\n", argv[0]);
    exit(1);
  }
  
  filename = argv[1];
  myfile = fopen(filename, "r");
  
  fseek(myfile, 0, SEEK_END);
  length = ftell(myfile);
  fseek(myfile,0, SEEK_SET);
  
  filecontent = (char*)malloc(sizeof(char) * (length +1));
  
  count = 0;
  
  //reads the file content
  int spaces = 0;
  while((filecontent[count] = fgetc(myfile)) != EOF) {
    if(filecontent[count] == ' ' || filecontent[count] == '\n')spaces++;
    count++;
  }
  filecontent[count-1] = '\0';
  
  //find the substrings
  
  ret = strtok(filecontent, " ");
  count = 0;
  
  array = (char**)malloc(sizeof(char*) * spaces);
  
  do {
    array[count] = (char*)malloc(sizeof(char) * strlen(ret));
    strcpy(array[count++], ret);
    //printf("%s\n", ret);
   
  }while((ret = strtok(NULL, " ")) != NULL);
    
 // printf("%s\n", filecontent);
  for(int i =0; i < spaces; i++)
    printf("%s \n", array[i]);
      
  //printf("length %d\n", length);
  for(int i =0; i < spaces; i++)
    free(array[i]);
  free(array);
  free(filecontent);
  
  fclose(myfile);
  return 0;
  
}