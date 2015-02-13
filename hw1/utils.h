#ifndef _UTILS_H
#define _UTILS_H
#endif

struct responsetime {
  int seconds;
  double milliseconds;
};

int createArrayfromFile(char *filename, char ***array_);
void calculateDeltaTime(struct timeval start, struct timeval end, struct responsetime *res);

