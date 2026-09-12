
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


int zeroread(struct inode *ip, char *dst, int n) {
    iunlock(ip);
    memset(dst, 0, n);
    ilock(ip);
    return n;
}

int zerowrite(struct inode *ip, char *buf, int n) {
    iunlock(ip);
    ilock(ip);
    return n;
}

void zeroinit(void) {
    devsw[ZERO].write = zerowrite;
    devsw[ZERO].read = zeroread;
}
