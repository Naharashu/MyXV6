#include "libc/stdlib.h"
#include "libc/stdio.h"
#include "user.h"

int main() {
    srand(123);
    for(int i = 0;i<10;i++) printf(1, "random number %d: %u\n", i, rand());
    exit();
}