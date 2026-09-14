#include "libc/stdint.h"
#include "types.h"
#include "stat.h"
#include "user.h"


void size(const char* filename) {
    struct stat s;
    stat(filename, &s);
    printf(STDIN, "%u bytes (%f MB)\n", s.size, ((double)s.size / (1000.0 * 1000.0)));
}

int main(int argc, char *argv[]) {
    int fd, i;

    for (i = 1; i < argc; i++) {
        if ((fd = open(argv[i], 0)) < 0) {
            printf(1, "size: cannot open %s\n", argv[i]);
            exit();
        }
        size(argv[i]);
    }
    exit();
}
