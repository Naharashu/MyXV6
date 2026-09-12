
#include "types.h"
#include "defs.h"
#include "sleeplock.h"
#include "spinlock.h"
#include "fs.h"
#include "file.h"
#include "memlayout.h"
#include "mmu.h"
#include "param.h"
#include "proc.h"
#include "traps.h"
#include "x86.h"


int nullread(struct inode *ip, char *dst, int n) {
    iunlock(ip);
    ilock(ip);
    return n;
}

int nullwrite(struct inode *ip, char *buf, int n) {
    iunlock(ip);
    ilock(ip);
    return n;
}

void nullinit(void) {
    devsw[DEVNULL].write = nullwrite;
    devsw[DEVNULL].read = nullread;
}
