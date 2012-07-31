#include "types.h"

inline void outb(u16 port, u8 data)
{
	asm volatile("outb %0, %1" : : "a" (data), "Nd" (port));
}

inline u8 inb(u16 port)
{
	u8 ret;
	asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

inline void io_wait()
{
	outb(0x80, 0);
}

