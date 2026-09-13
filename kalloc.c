// Physical memory allocator, intended to allocate
// memory for user processes, kernel stacks, page table pages,
// and pipe buffers. Allocates 4096-byte pages.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "spinlock.h"

void freerange(void *vstart, void *vend);
extern char end[]; // first address after kernel loaded from ELF file
                   // defined by the kernel linker script in kernel.ld

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  int use_lock;
  struct run *freelist;
} kmem;

// Initialization happens in two phases.
// 1. main() calls kinit1() while still using entrypgdir to place just
// the pages mapped by entrypgdir on free list.
// 2. main() calls kinit2() with the rest of the physical pages
// after installing a full page table that maps them on all cores.
void
kinit1(void *vstart, void *vend)
{
  initlock(&kmem.lock, "kmem");
  kmem.use_lock = 0;
  freerange(vstart, vend);
}

void
kinit2(void *vstart, void *vend)
{
  freerange(vstart, vend);
  kmem.use_lock = 1;
}

void
freerange(void *vstart, void *vend)
{
  char *p;
  p = (char*)PGROUNDUP((uint)vstart);
  for(; p + PGSIZE <= (char*)vend; p += PGSIZE)
    kfree(p);
}
//PAGEBREAK: 21
// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(char *v)
{
  struct run *r;

  if((uint)v % PGSIZE || v < end || V2P(v) >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(v, 1, PGSIZE);

  if(kmem.use_lock)
    acquire(&kmem.lock);
  r = (struct run*)v;
  r->next = kmem.freelist;
  kmem.freelist = r;
  if(kmem.use_lock)
    release(&kmem.lock);
}



// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
char*
kalloc(void)
{
  struct run *r;

  if(kmem.use_lock)
    acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;
  if(kmem.use_lock)
    release(&kmem.lock);
  return (char*)r;
}

// Allocate n physically contiguous pages.
// Returns the address of the first page, or 0 if no such block exists.
char*
kmalloc(int n)
{
  struct run *candidate, *current, *previous;
  struct run *pages[n];
  int i, found;

  if(n <= 0)
    panic("kmalloc");

  if(kmem.use_lock)
    acquire(&kmem.lock);

  found = 0;
  for(candidate = kmem.freelist; candidate && !found;
      candidate = candidate->next){
    found = 1;
    for(i = 0; i < n; i++){
      pages[i] = 0;
      for(current = kmem.freelist; current; current = current->next){
        if((char*)current == (char*)candidate + i * PGSIZE){
          pages[i] = current;
          break;
        }
      }
      if(pages[i] == 0){
        found = 0;
        break;
      }
    }
  }

  if(!found) {
    if(kmem.use_lock)
      release(&kmem.lock);
    return 0;
  }

  // Remove all pages while holding the lock, so another CPU cannot take one.
  for(i = 0; i < n; i++){
    previous = 0;
    for(current = kmem.freelist; current != pages[i];
        current = current->next)
      previous = current;
    if(previous)
      previous->next = current->next;
    else
      kmem.freelist = current->next;
  }

  if(kmem.use_lock)
    release(&kmem.lock);

  return (char*)pages[0];
}

