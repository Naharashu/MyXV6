#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
    if(argc<3) {
        printf(2, "usage: rename oldname newname\n");
    }
    if (rename(argv[1], argv[2]) < 0) {
        printf(2, "rename failed\n");
    }
    exit();
}