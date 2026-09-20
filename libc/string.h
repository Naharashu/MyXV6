#ifndef STRING_H
#define STRING_H

typedef unsigned char uchar;
typedef unsigned int uint;

char *strcpy(char *s, const char *t);

int strcmp(const char *p, const char *q);

uint strlen(const char *s);

void *memset(void *dst, int c, uint n);
char *strchr(const char *s, char c);


void *memmove(void *vdst, const void *vsrc, int n);


int memcmp(const void *v1, const void *v2, uint n);


// memcpy exists to placate GCC.  Use memmove.
void *memcpy(void *dst, const void *src, uint n);

int strncmp(const char *p, const char *q, uint n);
char *strncpy(char *s, const char *t, int n);

// Like strncpy but guaranteed to NUL-terminate.
char *strcpy_n(char *s, const char *t, int n);

// Source - https://stackoverflow.com/a/2490637
// Posted by unwind, modified by community. See post 'Timeline' for change history
// Retrieved 2026-09-17, License - CC BY-SA 2.5

char *strcat(char *dest, const char *src);

#endif