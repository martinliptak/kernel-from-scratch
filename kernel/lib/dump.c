#include "types.h"

void xdump(void *data, int count)
{
	while (count--)
		printk("%x ", *(u8 *)data++);
	printk("\n");
}

void bdump(void *data, int count)
{
	while (count--)
		printk("%b ", *(u8 *)data++);
	printk("\n");
}

