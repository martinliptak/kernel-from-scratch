#include "types.h"
#include "pic.h"

void pic_init()
{
	u8 a1, a2;

	a1 = inb(0x21);		// Save masks
	a2 = inb(0xa1);

	outb(0x20, 0x11);	// Start the initialization sequence
	io_wait();
	outb(0xa0, 0x11);
	io_wait();
	outb(0x21, 0x20);	// IRQ 0..7 -> INTR 0x20..0x27
	io_wait();
	outb(0xa1, 0x28);	// IRG 8..15 -> INTR 0x28..0x2f
	io_wait();
	outb(0x21, 4);
	io_wait();
	outb(0xa1, 2);
	io_wait();
	outb(0x21, 1);
	io_wait();
	outb(0xa1, 1);
	io_wait();

	outb(0x21, a1); 	// Restore masks
	outb(0x21, a2);
}

void pic_unmask(u8 irq)
{
	lock();
	if (irq > 7)
		outb(0xa1, inb(0xa1) & ~(irq - 8 + 1));
	else
		outb(0x21, inb(0x21) & ~(irq + 1));
	unlock();
}

void pic_unlock(bool slave)
{
	outb(0x20, 0x20);
	if (slave)
		outb(0xa0, 0x20);
}

