#include <types.h>
#include <kernel.h>
#include <memory.h>

bool page_map_4k(pgdir_t pgdir, ulong virt, ulong phys, page_flag_t flags)
{
	ulong *pde, *pte;
	
	lock();
	pde = pgdir->virt + (virt >> 22);
	if (!(*pde & PAGE_PRESENT))
	{
		*pde = phys_alloc(PA_KERNEL);
		if (!*pde)
		{
			unlock();
			return false;
		}
		*pde |= PAGE_PRESENT | PAGE_WRITE;
	}

	pagedir[1023] = pgdir[1022];
	pte = (ulong *)((0xffc00000 | (virt >> 10) & 0xfffff000) 
		| (0x00000fff & (virt >> 12)) * 4);
	*pte = flags & 0x00000fff;
	*pte |= phys & 0xfffff000;
	
	unlock();
	return true;
}

bool page_map_4m(ulong pgdir[], ulong virt, ulong phys, page_flag_t flags)
{
	ulong *pde;

	lock();
	pde = pgdir + (virt >> 22);
	*pde = flags & 0x00000fff;
	*pde |= phys & 0xffc00000;
	unlock();

	return true;
}

