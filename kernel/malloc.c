#include "types.h"
#include "memory.h"

#define HEAP_MAGIC 0x7ABCDEF7

typedef struct _malloc_block
{
	unsigned		magic : 31;
	unsigned		used : 1;
	ulong			size;
	struct _malloc_block	*next;
} malloc_block_t;

static malloc_block_t	*heap_top = HEAP_START;
static malloc_block_t	*heap_bottom = HEAP_START;
static malloc_block_t	block_head = {HEAP_MAGIC, true, 0, NULL};

/* Create a free block of at least "size" bytes. It allocates the nearest 
   greater multiply of 4096 bytes. It is similar to sbrk(). */
static malloc_block_t *more_heap(ulong size)
{
	ulong		page, count, i;
	malloc_block_t	*original_top;

	/* heap_top is increased later. We will put malloc_block_t to the beginning
	  of the original top and it will be the free block of at least "size" bytes
	  we want to return. */
	original_top = heap_top;
	/* Number of pages needed to allocate. */
	count = (size + sizeof(malloc_block_t) % 4096 == 0) 
			? (size + sizeof(malloc_block_t)) / 4096 
			: (size + sizeof(malloc_block_t)) / 4096 + 1;

	for (i = 0; i < count; ++i)
	{
		page = phys_alloc(PA_KERNEL);
		if (page == 0)
		{
			bug("malloc(): Can't allocate physical pages for kernel heap.");
			break;
		}
		page_map_4k(pagedir, (ulong)heap_top, page, PAGE_WRITE | PAGE_PRESENT);
		/* heap_top is increased each time we allocate a new page. Next time
		   more_heap() is called it will use the new heap_top as original_top. */
		heap_top = (malloc_block_t *)((char *)heap_top + 4096);
	}
	
	/* i is the number of allocated pages. It might not be enough (lower than
	   "size"), because this function does not call panic(), when the system is
	   out of memory. This is checked in malloc(), where this function is called 
	   from */
	original_top->size = 4096 * i - sizeof(malloc_block_t);
	original_top->used = false;
	original_top->magic = HEAP_MAGIC;
	original_top->next = NULL;
	
	return original_top;
}

/* Makes one free block from more free contigous blocks. */
static inline void merge_free_blocks(malloc_block_t *blk)
{
	while (blk->used == false && blk->next && blk->next->used == false)
	{
		if (blk->magic != HEAP_MAGIC)
		{
			panic("Heap corrupted");
			return;
		}
		blk->size += blk->next->size + sizeof(malloc_block_t);
		blk->next = blk->next->next;
	}
}

/* Make two smaller blocks from larger one. One of the new blocks is marked as 
   used and one is free. */
static malloc_block_t *cut_block(malloc_block_t *blk, ulong size)
{
	malloc_block_t *nblk;

	/* A block with a required size fits in the "blk" block. There is also
	   space for malloc_block_t and a new free block. */
	if (blk->size > size + sizeof(malloc_block_t))
	{
		nblk = (malloc_block_t *)((char *)blk + sizeof(malloc_block_t) + size);
		
		nblk->size = blk->size - size - sizeof(malloc_block_t);
		nblk->used = false;
		nblk->magic = HEAP_MAGIC;
		nblk->next = blk->next;
		/* A new free block is created. There might be a free block
		   next to the it. */
		merge_free_blocks(nblk);
		
		blk->size = size;
		blk->used = true;
		blk->next = nblk;

		return (malloc_block_t *)((char *)blk + sizeof(malloc_block_t));
	}
	/* There is a space for the required size, but the malloc_block_t does
	   not fit in, so a new free block can not be created. */
	else if (blk->size >= size && blk->size <= size + sizeof(malloc_block_t))
	{
		blk->used = true;
		
		return (malloc_block_t *)((char *)blk + sizeof(malloc_block_t));
	}
	else
		/* This should never happen. */
		return NULL;

}

void malloc_dump_heap()
{
	malloc_block_t *blk;

	for (blk = block_head.next; blk; blk = blk->next)
	{
		if (blk->magic != HEAP_MAGIC)
		{
			panic("Heap corrupted");
			return;
		}
		printk("size: %6u %s next: %10x\n", blk->size, blk->used ? 
				"used" : "free", blk->next);
	}
}

void *malloc(ulong size)
{
	malloc_block_t	*blk, *ret;
	ulong		page;
	void		*retval;
	
	if (size == 0)
		return NULL;

	lock();
	for (blk = &block_head; blk->next; blk = blk->next)
	{
		if (blk->magic != HEAP_MAGIC)
			panic("Heap corrupted");
		/* "blk" is not used and a block of required size fits in. */
		if (!(blk->used || blk->size < size))
		{
			retval = cut_block(blk, size);
			goto ret;
		}
	}
	if (blk->magic != HEAP_MAGIC)
	{
		panic("Heap corrupted");
		retval = NULL;
		goto ret;
	}
	/* This is for the last block. If you have a better idea how to do it, tell
	   me. After the end of the for loop I need "blk" to be the last block. */
	if (!(blk->used || blk->size < size))
	{	
		retval = cut_block(blk, size);
		goto ret;
	}
	
	if (blk->used)
	{
		blk->next = more_heap(size);
		if (blk->next->size < size)
		{
			/* more_heap() may allocate less memory than required 
			   when the system is out of memory. We will fail and 
			   use the amount of memory which is insufficient now 
			   later. */
			retval = NULL;
		}
		else
		{
			/* more_heap() may allocate more memory than required
			   (see more_heap()). We will divide the block it 
			   returns to the one we need and to a free block which
			   will be used later. */
			retval = cut_block(blk->next, size);
		}
	}
	else
	{
		/* The block at the end is free, so it can be merged with the 
		   newly created block. We can allocate less memory. */
		blk->next = more_heap(size - blk->size - sizeof(malloc_block_t));
		merge_free_blocks(blk);
		/* blk->next is NULL, because we merged the last blocks. */
		if (blk->size < size)
			retval = NULL;
		else
			retval = cut_block(blk, size);
	}
ret:
	unlock();
	return retval;
}

void free(void *addr)
{
	malloc_block_t	*blk, *fblk;

	/* "addr" is not the address of a block, but the address of the 
	   first usable byte after malloc_block_t. */
	fblk = addr - sizeof(malloc_block_t);
	for (blk = block_head.next; blk; blk = blk->next)
	{
		if (blk->magic != HEAP_MAGIC)
		{
			panic("Heap corrupted");
			return;
		}
		if (blk == fblk)
			goto found;
	}
	bug("free(): Trying to free a block which is not allocated");
	return;
found:
	if (blk->used == false)
	{
		bug("free(): Trying to free a block twice");
		return;
	}
	blk->used = false;
	/* We need to merge the free blocks before the newly created 
	   block as well as after. merge_free_blocks() merges only after, 
	   because you can not move backwards in the block list. */
	 for (blk = block_head.next; blk; blk = blk->next)
		merge_free_blocks(blk);
}

void *realloc(void *addr, ulong size)
{
	malloc_block_t	*blk, *fblk;
	void 		*naddr;

	fblk = addr - sizeof(malloc_block_t);
	for (blk = block_head.next; blk; blk = blk->next)
	{
		if (blk->magic != HEAP_MAGIC)
		{
			panic("Heap corrupted");
			return;
		}
		if (blk == fblk)
			goto found;
	}
	bug("realloc(): Trying to reallocate a block which is not allocated");
	return NULL;
found:
	if (blk->used == false)
	{
		bug("realloc(): Trying to reallocate a block which is not allocated");
		return NULL;
	}
	
	if (size == 0)
	{
		free(addr);
		return NULL;
	}
	else if (blk->size == size)
	{
		return blk;
	}
	else if (blk->size > size)
	{
		/* shrinking */
		return cut_block(blk, size);
	}
	else
	{
		/* enlarging */
		/* "blk" might be the last block. In that case we should free 
		   it at first and its memory can be reused in the forthcoming
		   malloc(). */
		free(addr);
		naddr = malloc(size);
		/* The new block may be same as the last block, but enlarged. */
		if (addr != naddr)
			memcpy(naddr, addr, blk->size);
		
		return naddr;
	}
}

