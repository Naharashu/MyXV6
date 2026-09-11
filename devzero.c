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


int zeroread(struct inode *ip, char *dst, int n) {
  memset(dst, 0, n);
  return n;
}

int zerowrite(struct inode *ip, char *buf, int n) {
  return n;
}

void zeroinit(void) {
  devsw[ZERO].write = zerowrite;
  devsw[ZERO].read = zeroread;
}
