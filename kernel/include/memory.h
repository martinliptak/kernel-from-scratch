#ifndef _MEMORY_H
#define _MEMORY_H

#include "types.h"

/*
   Virtual memory
   
   START	END		LENGTH	DESCRIPTION
   0 		0xbfffffff	3 GB	user space
   0xc0000000 	0xc0ffffff 	16 MB	four 4MB pages mapped at 0, 4MB, 8MB and 16 MB
   0xc1000000 	0xc13fffff	4 MB	kaddr structures
   0xc1400000 	0xdfffffff	492 MB	can be allocated using kaddr and used 
   					for any purpose
   0xe0000000 	0xffffefff	508 MB	kernel heap
   0xfffff000 	0xffffffff	4 MB 	page directory is allocated into itself, 
   					used to access page tables
*/

/* Use these macros when you access physical memory pointers to a memory 
   under 16 MB */
#define VIRTUAL_BASE 		0xc0000000
#define PHYS_TO_VIRT(addr) 	(void *)((char *)(addr) + VIRTUAL_BASE)
#define VIRT_TO_PHYS(addr) 	(void *)((char *)(addr) - VIRTUAL_BASE)

/* Used in kaddr.c */
#define KADDR_STRUCTS 	(void *)0xc1000000
#define KADDR_START 	(void *)0xc1400000
#define KADDR_END 	(void *)0xdfffffff

/* Used in malloc.c */
#define HEAP_START	(void *)0xe0000000
#define HEAP_END	(void *)0xffffefff

/* Flags to page_map_*() */
typedef enum
{
	PAGE_PRESENT 	= 1 << 0, 
	PAGE_WRITE	= 1 << 1, 
	PAGE_USER	= 1 << 2, 
	PAGE_PWT	= 1 << 3, 
	PAGE_PCD	= 1 << 4, 
	PAGE_ACCESSED	= 1 << 5, 
	PAGE_SIZE	= 1 << 7
} page_flag_t;

/* Flags to phys_alloc(), but not implemented yes */
typedef enum
{
	PA_KERNEL	= 1 << 0, 
	PA_INTERRUPT	= 1 << 1, 
	PA_USER		= 1 << 2, 
	PA_ISADMA	= 1 << 3
} phys_alloc_flag_t;

typedef struct
{
	ulong *phys;
	ulong *virt;
} pgdir_t;

/*** Defined in boot.asm ***/

extern ulong pagedir[1024];

/*** Defined in phys.c ***/

void 	phys_init();
/* Allocate one page of physical memory. May return 0 when the system is out 
   of memory. */
ulong 	phys_alloc(phys_alloc_flag_t flags);
void 	phys_free(ulong page);

/*** Defined in kaddr.c ***/

/* Allocate number of pages of virtual address space. May return NULL when 
   a suitable block could not be created or found. The allocated range can 
   be used for example for buffers or caches. */ 
void	*kaddr_alloc(ulong pages);
void 	kaddr_free(void *base);

/*** Defined in page.c ***/

/* Maps virtual pages to physical pages. Creates a page table if it is neeeded. */
bool 	page_map_4k(ulong pgdir[], ulong virt, ulong phys, page_flag_t flags);
bool 	page_map_4m(ulong pgdir[], ulong virt, ulong phys, page_flag_t flags);

/*** Defined in malloc.c ***/

/* Behaves like standard C functions. */
void 	*malloc(ulong size);
void 	free(void *addr);
void 	*realloc(void *addr, ulong size);
/* Prints all blocks on the heap. Does not print their contents, 
   only size, used/free and the address of next block. */
void 	malloc_dump_heap();

#endif

