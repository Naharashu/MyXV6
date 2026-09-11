#include "types.h"
#include "user.h"
#include "fcntl.h"

int
main(void)
{
    int f = open("/dev/zero", O_RDONLY);

    if(f < 0){
        printf(2, "open /dev/zero failed\n");
        exit();
    }

    char *buf = malloc(12);

    for(int i = 0; i < 12; i++)
        buf[i] = 1;

    int n = read(f, buf, 12);

    printf(2, "read returned %d\n", n);

    for(int i = 0; i < 12; i++)
        printf(2, "%d ", buf[i]);

    printf(2, "\n");

    close(f);
    free(buf);
    exit();
}