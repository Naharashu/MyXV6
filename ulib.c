#include "fcntl.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "x86.h"


char *gets(char *buf, int max) {
    int i, cc;
    char c;

    for (i = 0; i + 1 < max;) {
        cc = read(0, &c, 1);
        if (cc < 1)
            break;
        buf[i++] = c;
        if (c == '\n' || c == '\r')
            break;
    }
    buf[i] = '\0';
    return buf;
}



int atoi(const char *s) {
    int n;

    n = 0;
    while ('0' <= *s && *s <= '9')
        n = n * 10 + *s++ - '0';
    return n;
}

long atol(const char *s) {
    long n;

    n = 0;
    while ('0' <= *s && *s <= '9')
        n = n * 10 + *s++ - '0';
    return n;
}

long long atoll(const char *s) {
    long long n;

    n = 0;
    while ('0' <= *s && *s <= '9')
        n = n * 10 + *s++ - '0';
    return n;
}


/* reboot provided by usys.S assembly stub */