#ifndef _BUFFER_H
#define _BUFFER_H

#include "types.h"

typedef void (*buffer_handler_t)(uint);

typedef struct
{
	uint			*len;
	buffer_handler_t	handler;
} buffer_t;

/*** buffer.c ***/

/*
   You can use buffer_register() to register a buffer. The kernel is in an 
   infinite loop and during each iteration it checks the lenghts (buffer_t.len)
   of all registered buffers and calls handlers (buffer_t.handler) when they 
   are nonzero. 
   This is used for example in the keyboard driver. Keyboard ISR puts
   a scancode to a buffer and increases the buffer length variable. Then
   the kernel finds out that the variable is nonzero and calls the handler.
   IMPORTANT: It can return 0, when there is no space for another buffer
   information.
*/
unsigned int buffer_register(buffer_handler_t handler, uint *len);
// Used in infinite kernel loop. 
void buffer_call_handlers();

#endif

