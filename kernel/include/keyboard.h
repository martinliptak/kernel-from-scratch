#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "types.h"

typedef enum
{
	KEY_ERROR, 
	KEY_ESC, 
	KEY_LCTRL, 
	KEY_LSHIFT, 
	KEY_RSHIFT, 
	KEY_LALT, 
	KEY_CAPS, 
	KEY_NUM, 
	KEY_SCROLL, 
	KEY_F1, 
	KEY_F2, 
	KEY_F3, 
	KEY_F4, 
	KEY_F5, 
	KEY_F6, 
	KEY_F7, 
	KEY_F8, 
	KEY_F9, 
	KEY_F10, 
	KEY_NUM, 
	KEY_SCROLL, 
	KEY_HOME, 
	KEY_UP, 
	KEY_PAGEUP, 
	KEY_LEFT, 
	KEY_RIGHT, 
	KEY_END, 
	KEY_DOWN, 
	KEY_PAGEDOWN, 
	KEY_INS, 
	KEY_DEL, 
	KEY_F11, 
	KEY_F12, 
	KEY_RCTRL, 
	KEY_RALT
} special_key_t;

typedef struct
{
	int	type; 
	char 	value;
	char 	shift;
	char 	escaped;
} key_t;

extern key_t 	keymap[256];
extern u8	kbd_buf[256];
extern uint	kbd_buf_len;

#endif
