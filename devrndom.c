
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

static uint state;
static struct spinlock rnd_lock;

static uint random_seed(void) {
    uint lo, hi;

    asm volatile("rdtsc" : "=a"(lo), "=d"(hi));

    return lo ^ hi;
}

static void init_xorshift32(void) {
    initlock(&rnd_lock, "rnd");
    state = random_seed();
    state ^= random_seed();
    state = (state << 13) ^ (random_seed() >> 5);
    if (state == 0)
        state = 0x12345678;
}

static uint xorshift32(void) {
    uint x;
    acquire(&rnd_lock);
    x = state;
    x = x ^ (x << 13);
    x = x ^ (x >> 17);
    x = x ^ (x << 5);
    state = x;
    release(&rnd_lock);
    return x;
}

int rndread(struct inode *ip, char *dst, int n) {
    int target = n;

    iunlock(ip);

    while (n >= 4) {
        if (myproc()->killed) {
            ilock(ip);
            return -1;
        }
        uint r = xorshift32();

        dst[0] = r;
        dst[1] = r >> 8;
        dst[2] = r >> 16;
        dst[3] = r >> 24;

        dst += 4;
        n -= 4;
    }

    if (n > 0) {
        uint r = xorshift32();

        while (n > 0) {
            if (myproc()->killed) {
                ilock(ip);
                return -1;
            }
            *dst++ = r;
            r >>= 8;
            n--;
        }
    }

    ilock(ip);

    return target;
}

int rndwrite(struct inode *ip, char *buf, int n) {
    iunlock(ip);
    ilock(ip);
    return -1;
}

void rndominit(void) {
    init_xorshift32();
    devsw[RNDOM].write = rndwrite;
    devsw[RNDOM].read = rndread;
}
