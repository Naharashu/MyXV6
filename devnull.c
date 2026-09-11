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



int nullread(struct inode *ip, char *dst, int n) {
  return n;
}

int nullwrite(struct inode *ip, char *buf, int n) {
  return n;
}

void nullinit(void) {
  devsw[DEVNULL].write = nullwrite;
  devsw[DEVNULL].read = nullread;
}

