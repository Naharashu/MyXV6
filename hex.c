#include "types.h"
#include "user.h"

#include "stat.h"
#include "types.h"
#include "user.h"

char buf[512];

void hex(int fd) {
    int n;

    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        for (int i = 0; i < n; i++) {
            printf(STDIN, "%x ", (uchar)buf[i]);
        }
    }
    if (n < 0) {
        printf(STDERR, "hex: read error\n");
        exit();
    }
    printf(STDIN, "\n");
}

int main(int argc, char *argv[]) {
    int fd, i;

    if (argc <= 1) {
        hex(0);
        exit();
    }

    for (i = 1; i < argc; i++) {
        if ((fd = open(argv[i], 0)) < 0) {
            printf(STDERR, "hex: cannot open %s\n", argv[i]);
            exit();
        }
        hex(fd);
        close(fd);
    }
    exit();
}
