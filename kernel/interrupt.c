#include "types.h"
#include "interrupt.h"

isr_t isr_0 = unexpected_interrupt;
isr_t isr_6 = unexpected_interrupt;
isr_t isr_8 = unexpected_interrupt;
isr_t isr_10 = unexpected_interrupt;
isr_t isr_11 = unexpected_interrupt;
isr_t isr_12 = unexpected_interrupt;
isr_t isr_13 = unexpected_interrupt;
isr_t isr_14 = unexpected_interrupt;
isr_t isr_16 = unexpected_interrupt;

// isr_t isr_32 = unexpected_interrupt;
isr_t isr_33 = unexpected_interrupt;

void irq_on()
{
	asm("sti");
}

void irq_off()
{
	asm("cli");
}

bool irq_stat()
{
	int f;
	asm volatile("pushf; popl %0" : "=g"(f));
	return f & (1 << 9);
}

void set_interrupt(u8 vector, void *handler)
{
	u8 *desc = idt + vector * 8;

	lock();
	((u16 *)desc)[0] = (u32)handler & 0xffff;
	((u16 *)desc)[1] = 0x08;
	desc[4] = 0;
	desc[5] = 0x8e;
	((u16 *)desc)[3] = (u32)handler >> 16;
	unlock();
}

void set_isr_wrappers()
{
	extern void (*isr_0_wrapper)();
	extern void (*isr_6_wrapper)();
	extern void (*isr_8_wrapper)();
	extern void (*isr_10_wrapper)();
	extern void (*isr_11_wrapper)();
	extern void (*isr_12_wrapper)();
	extern void (*isr_13_wrapper)();
	extern void (*isr_14_wrapper)();
	extern void (*isr_16_wrapper)();
	extern void (*isr_32_wrapper)();
	extern void (*isr_33_wrapper)();

	set_interrupt(0, &isr_0_wrapper);
	set_interrupt(6, &isr_6_wrapper);
	set_interrupt(8, &isr_8_wrapper);
	set_interrupt(10, &isr_10_wrapper);
	set_interrupt(11, &isr_11_wrapper);
	set_interrupt(12, &isr_12_wrapper);
	set_interrupt(13, &isr_13_wrapper);
	set_interrupt(14, &isr_14_wrapper);
	set_interrupt(16, &isr_16_wrapper);
	set_interrupt(32, &isr_32_wrapper);
	set_interrupt(33, &isr_33_wrapper);
}

void dump_regs(registers_t *regs)
{
	printk("Registers: \n"
		"eax = %-11x ebx = %-11x ecx = %-11x edx = %x\n"
		"ebp = %-11x esp = %-11x esi = %-11x edi = %x\n", 
		regs->eax, regs->ebx, regs->ecx, regs->edx, 
		regs->ebp, regs->esp, regs->esi, regs->edi);
}

void unexpected_interrupt(uint n, registers_t regs)
{
	dump_regs(&regs);
	panic("Unexpected interrupt %d", n);
}

