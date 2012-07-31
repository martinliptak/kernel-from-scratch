#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include "types.h"

typedef struct
{
	uint edi;
	uint esi;
	uint ebp;
	uint esp;
	uint ebx;
	uint edx;
	uint ecx;
	uint eax;
} registers_t;

typedef void (*isr_t)(uint, registers_t);

/*** idt.asm ***/

extern u8 idt[256 * 8];

/*** interrupt.c ***/

// sti
void irq_on();
// cli
void irq_off();
// Returns 1 if IRQs are enabled, otherwise 0
bool irq_stat();
// Sets handler as interrupt gate in IDT. 
void set_interrupt(u8 vector, void *handler);
// Set asm wrappers for some interrupts
void set_isr_wrappers();
// Print values of the general purpose registers.
void dump_regs(registers_t *regs);
/* ISR that dumps the registers and prints the number of the unexpected 
   interrupt. The default ISR for all interrupts handled by the kernel. */
void unexpected_interrupt(uint n, registers_t regs);
/* Set these pointers to your own handlers. They will be called upon the
   appropriate interrupt. */
extern isr_t isr_0;
extern isr_t isr_6;
extern isr_t isr_8;
extern isr_t isr_10;
extern isr_t isr_11;
extern isr_t isr_12;
extern isr_t isr_13;
extern isr_t isr_14;
extern isr_t isr_16;
// extern isr_t isr_32;
extern isr_t isr_33;

#endif

