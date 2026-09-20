#include "string.h"


char *strcpy(char *s, const char *t) {
    char *os;

    os = s;
    while ((*s++ = *t++) != 0)
        ;
    return os;
}

int strcmp(const char *p, const char *q) {
    while (*p && *p == *q)
        p++, q++;
    return (uchar)*p - (uchar)*q;
}

uint strlen(const char *s) {
    int n;

    for (n = 0; s[n]; n++)
        ;
    return n;
}

void *memset(void *dst, int c, uint n) {
    unsigned char *bytes = dst;

    while (n-- > 0)
        *bytes++ = c;
    return dst;
}

char *strchr(const char *s, char c) {
    for (; *s; s++)
        if (*s == c)
            return (char *)s;
    return 0;
}


void *memmove(void *vdst, const void *vsrc, int n) {
    char *dst;
    const char *src;

    dst = vdst;
    src = vsrc;
    while (n-- > 0)
        *dst++ = *src++;
    return vdst;
}


int memcmp(const void *v1, const void *v2, uint n) {
    const uchar *s1, *s2;

    s1 = v1;
    s2 = v2;
    while (n-- > 0) {
        if (*s1 != *s2)
            return *s1 - *s2;
        s1++, s2++;
    }

    return 0;
}


// memcpy exists to placate GCC.  Use memmove.
void *memcpy(void *dst, const void *src, uint n) {
    return memmove(dst, src, n);
}

int strncmp(const char *p, const char *q, uint n) {
    while (n > 0 && *p && *p == *q)
        n--, p++, q++;
    if (n == 0)
        return 0;
    return (uchar)*p - (uchar)*q;
}

char *strncpy(char *s, const char *t, int n) {
    char *os;

    os = s;
    while (n-- > 0 && (*s++ = *t++) != 0)
        ;
    while (n-- > 0)
        *s++ = 0;
    return os;
}

// Like strncpy but guaranteed to NUL-terminate.
char *strcpy_n(char *s, const char *t, int n) {
    char *os;

    os = s;
    if (n <= 0)
        return os;
    while (--n > 0 && (*s++ = *t++) != 0)
        ;
    *s = 0;
    return os;
}

// Source - https://stackoverflow.com/a/2490637
// Posted by unwind, modified by community. See post 'Timeline' for change history
// Retrieved 2026-09-17, License - CC BY-SA 2.5

char *strcat(char *dest, const char *src) {
    char *rdest = dest;

    while (*dest)
        dest++;
    while (*dest++ = *src++)
        ;
    return rdest;
}