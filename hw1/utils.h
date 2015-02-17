#ifndef _UTILS_H
#define _UTILS_H
#endif

#define TRUE 1
#define FALSE 0
#define MAXLINES 500000
#define CHARS_PER_LINE 200
#define SIZE_OF_LINE sizeof(char) * CHARS_PER_LINE
#define MAXPROCESSES 16

void* array;
void* merged_array;
int total_processes;
int merge_direction;
int array_length;

struct responsetime {
  int seconds;
  double milliseconds;
};

struct thread_args {
  int arg1;
  int arg2;
  int arg3;
};

int createArrayfromFile(char *filename);
void calculateDeltaTime(struct timeval start, struct timeval end, struct timeval *res);
void swap(int i, int j);
int partition(int left, int right);
void quick_sort(int left, int right);
int isPowerOfTwo(int n);
int exponentOfPowerOf2(int n);
void change_merge_direction();
//mid contains the last vector's right session 
void merge(int left, int mid, int right, int merge_direction);


