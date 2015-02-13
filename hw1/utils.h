#ifndef _UTILS_H
#define _UTILS_H
#endif

#define MAXLINES 500000
#define CHARS_PER_LINE 200
#define SIZE_OF_LINE sizeof(char) * CHARS_PER_LINE

void* array;
void* merged_array;

struct responsetime {
  int seconds;
  double milliseconds;
};

int createArrayfromFile(char *filename);
void calculateDeltaTime(struct timeval start, struct timeval end, struct responsetime *res);
void swap(int i, int j);
int partition(int left, int right);
void quick_sort(int left, int right);


