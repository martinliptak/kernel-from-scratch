#include "console.h"
#include "multiboot.h"
#include "kernel.h"
#include "memory.h"
#include "library.h"
#include "timer.h"

multiboot_info_t *mb_info;

static void init()
{
	set_gdt();
	set_idt();
	set_isr_wrappers();
	phys_init();
	pic_init();
	cons_init();
	kbd_init();
	sched_init();
}

static void test_func1()
{
	while (1)
	{
		cons_putchar(cons, '1');
	}
}

static void test_func2()
{
	while (1)
	{
		cons_putchar(cons, '2');
	}
}

static void test()
{
	kthread_start(test_func1);
	// kthread_start(test_func2);

	while (1)
	{
		cons_putchar(cons, '0');
	}
}

void kmain(multiboot_info_t *mbi)
{
	register int old_ticks = 0;
	
	lock();
	mb_info = PHYS_TO_VIRT(mbi);
	cons_clear(cons);
	init();
	parse_cmdline(PHYS_TO_VIRT(mb_info->cmdline));
	unlock();
	
	printk("<1>Kernel boot successful\n");
	test();

	while (1)
	{
		buffer_call_handlers();
		if (timer_ticks != old_ticks && timer_ticks % TICKS_PER_SECOND == 0)
		{
			printk("Seconds: %d\n", timer_ticks / TICKS_PER_SECOND);
			old_ticks = timer_ticks;
		}
	}
}

