#include "keyboard.h"

key_t keymap[256] = {
	1, KEY_ERROR, 	0,	0, 
	1, KEY_ESC, 	0, 	0, 
	0, '1', 	'!',  	0,  
	0, '2', 	'@',   	0, 
	0, '3', 	'#',   	0, 
	0, '4', 	'$',   	0, 
	0, '5', 	'%',  	0, 
	0, '6', 	'^',   	0, 
	0, '7', 	'&',   	0, 
	0, '8', 	'*',   	0, 
	0, '9', 	'(',   	0, 
	0, '0', 	')',   	0, 
	0, '-', 	'_',   	0, 
	0, '=', 	'+',   	0, 
	0, '\b', 	'\b',  	0, 
	0, '\t', 	0,   	0, 
	0, 'q', 	'Q',   	0, 
	0, 'w', 	'W',   	0, 
	0, 'e', 	'E',   	0, 
	0, 'r', 	'R',   	0, 
	0, 't', 	'T',   	0, 
	0, 'y', 	'Y',   	0, 
	0, 'u', 	'U',   	0, 
	0, 'i', 	'I',   	0, 
	0, 'o', 	'O',   	0, 
	0, 'p', 	'P',   	0, 
	0, '[', 	'[',   	0, 
	0, ']', 	']',   	0, 
	0, '\n', 	0,   	'\n', 
	1, KEY_LCTRL, 	0,   	KEY_RCTRL, 
	0, 'a',		'A',   	0, 
	0, 's', 	'S',   	0, 
	0, 'd', 	'D',   	0, 
	0, 'f', 	'F',   	0, 
	0, 'g', 	'G',   	0, 
	0, 'h', 	'H',   	0, 
	0, 'j', 	'J',   	0, 
	0, 'k', 	'K',   	0, 
	0, 'l', 	'L',   	0, 
	0, ';', 	':',   	0, 
	0, '\'', 	'\"',   0, 
	0, '`', 	'~',   	0, 
	1, KEY_LSHIFT, 	0,   	0, 
	0, '\\', 	'|',   	0, 
	0, 'z', 	'Z',   	0, 
	0, 'x', 	'X',   	0, 
	0, 'c', 	'C',   	0, 
	0, 'v', 	'V',   	0, 
	0, 'b', 	'B',   	0, 
	0, 'n', 	'N',   	0, 
	0, 'm', 	'M',   	0, 
	0, ',', 	'<',   	0, 
	0, '.', 	'>',   	0, 
	0, '/', 	'?',   	'/', 
       	1, KEY_RSHIFT, 	0,   	0, 
	1, 0, 		0,  	0, 	// (Keypad-*) or (*/PrtScn) on a 83/84-key keyboard
	1, KEY_LALT, 	0,   	KEY_RALT, 
	0, ' ', 	' ',   	0, 
	1, KEY_CAPS, 	0,   	0, 
	1, KEY_F1, 	0,   	0, 
	1, KEY_F2, 	0,   	0, 
	1, KEY_F3, 	0,   	0, 
	1, KEY_F4, 	0,   	0, 
	1, KEY_F5, 	0,   	0, 
	1, KEY_F6, 	0,   	0, 
	1, KEY_F7, 	0,    	0, 
	1, KEY_F8,  	0,   	0, 
	1, KEY_F9,  	0,   	0, 
	1, KEY_F10,  	0,   	0, 
	1, KEY_NUM,  	0,   	0, 
	1, KEY_SCROLL,  0,   	0, 
	2, KEY_HOME, 	'7',   	KEY_HOME, 
	2, KEY_UP, 	'8',   	KEY_UP, 
	2, KEY_PAGEUP, 	'9',   	KEY_PAGEUP, 
	0, '-', 	'-',  	0, 
	2, KEY_LEFT, 	'4',   	KEY_LEFT, 
	0, '5', 	'5',  	0, 
	2, KEY_RIGHT, 	'6',   	KEY_RIGHT, 
	0, '+', 	'+',   	0, 
	2, KEY_END, 	'1',   	KEY_END, 
	2, KEY_DOWN, 	'2',   	KEY_DOWN, 
	2, KEY_PAGEDOWN, '3',   KEY_PAGEDOWN, 
	2, KEY_INS, 	'0',   	KEY_INS, 
	2, KEY_DEL, 	'.',   	KEY_DEL, 
	1, 0, 		0,  	0, 	// (Alt-SysRq) on a 84+ key keyboard
	1, 0, 		0,   	0, 
	1, 0, 		0,   	0, 
	1, KEY_F11, 	0,   	0, 
	1, KEY_F12, 	0,  	0 
};

