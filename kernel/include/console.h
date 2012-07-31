#ifndef _CONSOLE_H
#define _CONSOLE_H

#include "memory.h"
#include "types.h"

#define VIDEO_MEMORY PHYS_TO_VIRT(0xb8000)

typedef struct
{
	u8	*mem;
	int 	width;
	int	height;

	int	color;
	int	x;
	int 	y;

	int 	tabwidth;
} console_t;

/*** console.c ***/

/*
   Why console_t and cons0? Why isn't there a set of global variables for
   the console color, x and y? This looks strange, but it will be very useful 
   if we decide to write virtual consoles.
*/
extern console_t cons0;
extern console_t *cons;

// Updates the hardware cursor.
void cons_update(console_t *c);
void cons_move(console_t *c, int x, int y);
/* 
   Staring with console_t.mem sets every ever byte (ascii character) to 0 
   and every odd (attribute) to console_t.color.
*/
void cons_clear(console_t *c);
// It can specially deal with '\n', '\r', '\t', '\b'
void cons_putchar(console_t *c, const char chr);
// Console keyboard input handler.
void kbd_handler(uint index);
// Register the console input handler for the keyboard buffer
void cons_init();

#endif

