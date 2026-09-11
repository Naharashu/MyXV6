#include "types.h"
#include "user.h"
#include "fcntl.h"

#define DEFAULT_LINES 10
#define BUF_SIZE 512

void
head(int fd, int nlines, int nbytes)
{
    char buf[BUF_SIZE];
    int n, lines = 0, c = 0;

    while((n = read(fd, buf, nlines == -1 ? 1 : sizeof(buf))) > 0) {
        for(int i = 0; i < n; i++) {
            write(1, &buf[i], 1);
            if(nlines == -1) {
                if(++c >= nbytes) return;
            } else {
                if(buf[i] == '\n' && ++lines >= nlines) return;
            }
        }
    }
}

int
main(int argc, char *argv[])
{
    int fd, nlines = DEFAULT_LINES;
    int nbytes = 32;
    int filestart = 1;

    // check for -n flag
    if(argc > 1 && argv[1][0] == '-' && argv[1][1] == 'n') {
        nlines = atoi(argv[1] + 2);  // head -n10
        if(nlines <= 0) {
            printf(2, "head: invalid line count\n");
            exit();
        }
        filestart = 2;
    }

    if(argc > 1 && argv[1][0] == '-' && argv[1][1] == 'c') {
        nbytes = atoi(argv[1] + 2);  // head -c10
        if(nbytes <= 0) {
            printf(2, "head: invalid byte count\n");
            exit();
        }
        filestart = 2;
        nlines = -1;
    }

    if(filestart >= argc) {
        head(0, nlines, nbytes);
        exit();
    }

    for(int i = filestart; i < argc; i++) {
        if((fd = open(argv[i], O_RDONLY)) < 0) {
            printf(2, "head: cannot open %s\n", argv[i]);
            exit();
        }
        // print filename header if multiple files
        if(argc - filestart > 1)
            printf(1, "==> %s <==\n", argv[i]);
        head(fd, nlines, nbytes);
        close(fd);
    }

    exit();
}