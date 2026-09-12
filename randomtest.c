#include "types.h"
#include "user.h"

int
main(void)
{
    char buf[64];
    int n = open("/dev/urandom", 0);

    if(n < 0) {
        printf(1, "open failed\n");
        exit();
    }

    if(read(n, buf, sizeof(buf)) != sizeof(buf)) {
        printf(1, "read failed\n");
        exit();
    }

    for(int i = 0; i < sizeof(buf); i++)
        printf(1, "%x ", (uchar)buf[i]);

    printf(1, "\n");

    close(n);
    exit();
}
