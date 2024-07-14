// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct kmem{
  struct spinlock lock;
  struct run *freelist;
} kmem;

struct kmem kmems[NCPU];

void
kinit()
{
  push_off();
  int currentid = cpuid();
  pop_off();
  printf("cpuid: %d\n", currentid);
  
  for(int i=0; i<NCPU; i++){
    initlock(&kmems[i].lock, "kmem");
  }
  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;

  push_off();
  int currentid = cpuid();
  pop_off();

  acquire(&kmems[currentid].lock);
  trypushr(currentid, r);
  release(&kmems[currentid].lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;
  int issteal = 0;
  push_off();
  int currentid = cpuid();
  pop_off();

  acquire(&kmems[currentid].lock);
  r = trypopr(currentid);
  
  if(!r){
    for(int i = 0; i < NCPU; i++)
    {
      if(i != currentid && kmems[i].freelist){
        acquire(&kmems[i].lock);
        r = trypopr(i);
        trypushr(currentid, r);

        issteal = 1;
        release(&kmems[i].lock);
        break;
      }
    }
  }

  if(issteal)
    r = trypopr(currentid);

  release(&kmems[currentid].lock);

  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}

struct run* trypopr(int id){
  struct run *r;
  r = kmems[id].freelist;
  if(r)
    kmems[id].freelist = r->next;
  return r;
}

void trypushr(int id, struct run* r){
  if(r){
    r->next = kmems[id].freelist;
    kmems[id].freelist = r;
  }
  else
  {
    panic("cannot push null run");
  }
}
