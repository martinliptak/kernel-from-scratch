#include "sched.h"
#include "memory.h"

void kthread_exit();

process_t *kthread_start(void (*func)())
{
	ulong 		thr_pgdir_page, thr_stack_page;
	ulong 		*thr_pgdir, *thr_stack, *thr_stack_top;
	process_t	*thr;
	process_queue_t	*qthr;

	thr_pgdir_page = phys_alloc(PA_KERNEL);
	thr_stack_page = phys_alloc(PA_KERNEL);
	thr_pgdir = kaddr_alloc(1);
	thr_stack = kaddr_alloc(1);
	page_map_4k(pagedir, (ulong)thr_pgdir, thr_pgdir_page, PAGE_PRESENT | PAGE_WRITE);
	page_map_4k(pagedir, (ulong)thr_stack, thr_stack_page, PAGE_PRESENT | PAGE_WRITE);
	thr_stack_top = thr_stack + 1024;
	
	// ret
	*--thr_stack_top = (ulong)kthread_exit;
	// iret
	// eflags, cs, eip
	*--thr_stack_top = 0x246;
	*--thr_stack_top = 0x8;
	*--thr_stack_top = (ulong)func;
	// popad
	// eax, ecx, edx, ebx
	*--thr_stack_top = 0;
	*--thr_stack_top = 0;
	*--thr_stack_top = 0;
	*--thr_stack_top = 0;
	// esp
	--thr_stack_top;
        *thr_stack_top = (ulong)(thr_stack_top + 5);
	// ebp, esi, edi
	*--thr_stack_top = 0;
	*--thr_stack_top = 0;
	*--thr_stack_top = 0;
	// pop eax; mov cr3, eax
	// cr3
	memcpy(thr_pgdir, pagedir, 4096);
	thr_pgdir[1022] = thr_pgdir_page | PAGE_WRITE | PAGE_PRESENT;
	page_map_4k(thr_pgdir, VIRTUAL_BASE - 4096, thr_stack_page, 
			PAGE_PRESENT | PAGE_WRITE);

	thr = malloc(sizeof(process_t));
	memset(thr, 0, sizeof(thr));
	thr->esp = (ulong)VIRTUAL_BASE - ((ulong)thr_stack  + 4096
		- (ulong)thr_stack_top);
	thr->ss = 0x10;
	thr->cr3 = thr_pgdir_page;

	qthr = malloc(sizeof(process_queue_t));
	qthr->proc = thr;

	lock();
	qthr->next = run_queue->next;
	run_queue->next = qthr;
	unlock();
}

void kthread_exit()
{
	
}

