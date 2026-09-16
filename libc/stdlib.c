#include "stdlib.h"

int abs(int n)  {
    return n >= 0 ? n : -n;
}

long absl(long n)  {
    return n >= 0 ? n : -n;
}

unsigned int rand(void) {
    const unsigned int a = 1103515245u;
    const unsigned int m = 1u << 31;
    const unsigned int c = 12345u;
    seed = (a * seed + c) % m;
    return seed;
}

void srand(unsigned int start) {
    seed = start;
    return;
}