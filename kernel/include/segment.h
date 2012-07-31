#ifndef _SEGMENT_H
#define _SEGMENT_H

#include "types.h"

/*** gdt.asm ***/

extern u8 gdt[8192 * 8];

/*** segment.c ***/

/* 
   Creates a new segment descriptor in GDT and returns its selector. 
   It takes only 20 bits from the limit argument, the rest is ignored. 
   The type argument is copied to the sixth byte of the new descriptor. 
   The new descriptor is put on the first free descriptor in GDT. 
   The granularity is always set.
   The returned selector's RPL is always 0.
*/
u16 segment_new(u32 base, u32 limit, u8 type);

#endif

