// zero input and output.
// Input is from the keyboard or serial port.
// Output is written to the screen and serial port.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "traps.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"

static uint state;

static uint
random_seed(void)
{
    uint lo, hi;

    asm volatile("rdtsc" : "=a"(lo), "=d"(hi));

    return lo ^ hi;
}

static void init_xorshift32(void) {
  state  = random_seed();
  if(state == 0)
        state = 0x12345678;
}

static uint xorshift32(void) {
  uint x;

  x = state;
  x = x ^ (x << 13);
  x = x ^ (x >> 17);
  x = x ^ (x << 5);
  state = x;
  return x;
}

int
rndread(struct inode *ip, char *dst, int n)
{
    int target = n;

    while(n >= 4){
        if(myproc()->killed)
          return -1;
        uint r = xorshift32();

        dst[0] = r;
        dst[1] = r >> 8;
        dst[2] = r >> 16;
        dst[3] = r >> 24;

        dst += 4;
        n -= 4;
    }

    if(n > 0){
        uint r = xorshift32();

        while(n > 0){
            if(myproc()->killed)
              return -1;
            *dst++ = r;
            r >>= 8;
            n--;
        }
    }

    return target;
}

int rndwrite(struct inode *ip, char *buf, int n) {
  return n;
}

void rndominit(void) {
  init_xorshift32();
  devsw[RNDOM].write = rndwrite;
  devsw[RNDOM].read = rndread;
}

