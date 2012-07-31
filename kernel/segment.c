#include "types.h"
#include "segment.h"

u16 segment_new(u32 base, u32 limit, u8 type)
{
	u8 	*desc = NULL; 
	int	i;

	lock();
	for (i = 1; i < 8192; ++i)
		if (*(u32 *)(gdt + i * 8) == 0 && *(u32 *)(gdt + i * 8 + 4) == 0)
		{
			desc = gdt + i * 8;
			break;
		}
	if (desc == NULL)
		panic("Unable to create a new segment descriptor, GDT is full");
	
	((u16 *)desc)[0] = limit & 0xffff;
	((u16 *)desc)[1] = base & 0xffff;
	desc[4] = (base >> 16) & 0xff;
	desc[5] = type;
	desc[6] = (limit >> 16) & 0xf;
	desc[6] |= 0xc0;
	desc[7] = (base >> 24) & 0xff;

	unlock();
	return i << 3;
}

