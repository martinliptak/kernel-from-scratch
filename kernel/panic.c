#include "stdarg.h"

void reboot()
{
	outb(0x64, 0xfe);
}

void hang()
{
	irq_off();
	while (1)
		asm("hlt");
}

void panic(const char *format, ...)
{
	va_list arg;

	va_start(arg, format);
	printk("Kernel panic: ");
	vprintk(format, arg);
	printk("\n");
	va_end(arg);

	hang();
}

void warn(const char *format, ...)
{
	va_list arg;

	va_start(arg, format);
	printk("Kernel warning: ");
	vprintk(format, arg);
	printk("\n");
	va_end(arg);
}

void bug(const char *format, ...)
{
	va_list arg;

	va_start(arg, format);
	printk("Kernel bug: ");
	vprintk(format, arg);
	printk("\n");
	va_end(arg);
}

