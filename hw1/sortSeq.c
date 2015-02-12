/*CSC422 - HW1
 * Copyright (C) 2015 emanuelvalente@gmail.com
 * gcc sortSeq.c -o test -std=c99 or
 * gcc sortSeq.c -o test -std=gnu99 -DDEBUG=1 

 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifndef DEBUG
#define DEBUG 0
#endif

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
  
  //spaces is actually the length of the array
  return spaces;
  
}

void swap(char **array, int i, int j) {
  char *tmp;
  int len_i = strlen(array[i]);
  int len_j = strlen(array[j]);
  
  tmp = (char*)malloc(sizeof(char)* (len_i + 1));
  strcpy(tmp, array[i]);
  
  if(len_i < len_j) 
    array[i] = (char*)realloc(array[i], sizeof(char) * (len_j +1));
  
  else if(len_i > len_j) 
    array[j] = (char*)realloc(array[j], sizeof(char) * (len_i +1));
  
  strcpy(array[i], array[j]);
  strcpy(array[j], tmp); 
  
  free(tmp);
  
  
}

int partition(char **array, int left, int right) {
  int i, j;
  
  i = left;
  for(j = left + 1; j <= right; ++j) {
    if(strcmp(array[j], array[left]) < 0) {
      ++i;
      swap(array, i, j);
    }
  }
  
  swap(array, left, i);
  
  return i;
  
}

void quick_sort(char **array, int left, int right) {
  int r;
  
  if(right > left) {
    r = partition(array, left, right);
    
    quick_sort(array, left, r-1);
    quick_sort(array, r+1, right);    
  }
  
  
}
  

int main(int argc, char **argv) {
  
  char *filename;
  char **array;
  int array_length;
  
  if(argc < 2) {
    fprintf(stderr, "Usage: %s <input_file>\n\n", argv[0]);
    exit(1);
  }
  
  filename = argv[1]; 
  //creates the array
  array_length = createArrayfromFile(filename, &array);
  
  quick_sort(array, 0, array_length -1);
  
  
  //print the content case debug is set
  if(DEBUG) {
  for(int i =0; i < array_length; i++)
    printf("%s \n", array[i]);
      
  }
  
  //FREE memory from array
  for(int i =0; i < array_length; i++)
    free(array[i]);
  free(array);

  return 0;
  
}