#ifndef STDLIB_H
#define STDLIB_H

#include "../types.h"

extern void* malloc(uint nbytes);
extern void free(void* ap);
extern int atoi(const char* s);
extern int atol(const char* s);
extern int atoll(const char* s);

int abs(int n);
long absl(long n);

static unsigned int seed = 0;

unsigned int rand(void);
void srand(unsigned int start);

#endif