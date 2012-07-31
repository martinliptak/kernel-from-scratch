#ifndef _PIC_H
#define _PIC_H

#include "types.h"

/*** pic.c ***/

// Remaps both PICs to offsets 0x20 and 0x28. The masks are kept. 
void pic_init();
/* 
   Determines whether the passed IRQ is on the master PIC or slave. Then
   clears the mask bit of irq on the correct PIC.
*/
void pic_unmask(u8 irq);
// Sends 0x20 to the master or slave PIC command port.
void pic_unlock(bool slave);

#endif

