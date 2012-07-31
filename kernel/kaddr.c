#include "memory.h"

typedef struct _kaddr_block
{
	void 			*base;
	ulong 			pages;
	bool 			used;
	struct _kaddr_block 	*next;
} kaddr_block_t;

static kaddr_block_t kaddr_block_head = { KADDR_START, 0, true, NULL };

static ulong kaddr_structs_sz = 0;
static ulong kaddr_allocated_sz = 0;

void *kaddr_alloc(ulong pages)
{
	kaddr_block_t *block;

	lock();
	/* Find the last block. */
	for (block = &kaddr_block_head; block->next; block = block->next)
		;
	if (block->base + pages * 4096 >= KADDR_END)
	{
		/* A new block created after the last one would not fit to 
		   the address space reserved for kaddr. */
		for (block = &kaddr_block_head; block->next; block = block->next)
		{
			/* The first free block, which the required block 
			   would fit in to, is returned. */
			if (block->next->pages >= pages && block->next->used == false)
			{
				block->next->used = true;
				unlock();
				return block->next->base;
			}
		}
		unlock();
		return NULL;
	}
	else
	{
		/* A new block will fit in to the space for addr. */
		if (kaddr_structs_sz + sizeof(kaddr_block_t) > kaddr_allocated_sz)
		{
			/* Control structures will need more memory. */
			page_map_4k(pagedir, (ulong)KADDR_STRUCTS + kaddr_allocated_sz, 
					phys_alloc(PA_KERNEL), PAGE_WRITE | PAGE_PRESENT);
			kaddr_allocated_sz += 4096;
		}
		/* The last block will point to the new last block, which is 
		   the next block after it. */
		block->next = KADDR_STRUCTS + kaddr_structs_sz;
		kaddr_structs_sz += sizeof(kaddr_block_t);
		block->next->base = block->base + block->pages * 4096;
		block->next->pages = pages;
		block->next->used = true;
		block->next->next = NULL;
		
		unlock();
		return block->next->base;
	}
}

void kaddr_free(void *base)
{
	kaddr_block_t *block;
	bool found;

	lock();
	for (found = false, block = &kaddr_block_head; block->next; block = block->next)
	{
		if (block->next->base == base)
		{
			found = true;
			break;
		}
	}
	if (found)
	{
		if (block->next->used == false)
			bug("kaddr_free(): Trying to free a block twice.");
		else
			block->next->used = false;
	}
	else
		bug("kaddr_free(): Trying to free a block which is not allocated.");
	unlock();
}

