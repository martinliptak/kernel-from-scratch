#include "memory.h"
#include "multiboot.h"
#include "kernel.h"
#include "types.h"
#include "interrupt.h"

ulong *free_pages;	/* Points to the memory after the kernel bss. */
ulong *free_pages_ptr;	/* Points to the top of free_pages. */

void page_fault_isr(uint n, registers_t regs)
{
	uint 	cr2, cr3, err;

	asm volatile ("movl %%cr2, %%eax; movl %%eax, %0; movl %%cr3, %%eax; "
			"movl %%eax, %1" : "=g" (cr2), "=g" (cr3));
	err = *((uint *)&regs.eax + 1);

	if (cr2 == 0)
	{
		dump_regs(&regs);
		panic("Accessing address 0 (cr2 = %x cr3 = %x err = %x)", 
				cr2, cr3, err);
	}
	else
	{
		dump_regs(&regs);
		panic("Unexpected page-fault (cr2 = %x cr3 = %x err = %x)", 
				cr2, cr3, err);
	}
}

void phys_init()
{
	ulong i;
	ulong pages;
	ulong mem_upper;
	ulong mem_start;
       
	printk("<1>Upper memory: %d KB\n", mb_info->mem_upper);

	/* Only pages after 16 MB of physical memory are added to our stack */
	mem_start = 0x1000000; 
	mem_upper = (mb_info->mem_upper + 1024) * 1024;
	pages = mem_upper / 4096;

	if ((&_end - &_start + pages * 4) > mem_start)
	{
		panic("Kernel size and vm control information are larger than %d MB", 
				mem_start / 1024 / 1024);
	}

	/* Free pages will point to the memory after the kernel bss. The
	   first 16 MB of the physical memory is mapped starting at 
	   VIRTUAL_BASE. */
	free_pages = (ulong *)&_end;
	free_pages_ptr = (ulong *)&_end;
	
	for (i = mem_start / 4096; i < pages; ++i, ++free_pages_ptr)
		*free_pages_ptr = i * 4096;

	printk("<3>Pages count: %d\n", i - mem_start / 4096);
	isr_14 = page_fault_isr;
}

ulong phys_alloc(phys_alloc_flag_t flags)
{
	lock();
	if (free_pages_ptr == free_pages)
	{
		unlock();
		return 0;
	}
	else
	{
		unlock();
		return *--free_pages_ptr;
	}
}

void phys_free(ulong page)
{
	lock();
	*free_pages_ptr++ = page;
	unlock();
}

