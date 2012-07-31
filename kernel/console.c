#include "console.h"
#include "keyboard.h"

console_t cons0 = { (u8 *)VIDEO_MEMORY, 80, 25, 0x1f /* white on blue */, 0, 0, 8 };
console_t *cons = &cons0;

static int	shift = 0;
static bool	numlock = true;
static bool	capslock = false;
static bool	escaped = false;

void cons_update(console_t *c)
{
	u16 position = c->y * c->width + c->x;

	lock();
	outb(0x3D4, 0x0F);
	outb(0x3D5, (u8)position);
	outb(0x3D4, 0x0E);
	outb(0x3D5, (u8)(position>>8));
	unlock();
}

inline void cons_move(console_t *c, int x, int y)
{
	c->x = x;
	c->y = y;
	cons_update(c);
}

void cons_clear(console_t *c)
{
	int i;

	lock();
	for (i = 0; i < c->width * c->height * 2; i += 2)
	{
		c->mem[i] = 0;
		c->mem[i + 1] = c->color;
	}
	unlock();
	cons_move(c, 0, 0);
}

void cons_putchar(console_t *c, const char chr)
{
	int 	i;
	u8	*ptr;

	lock();
	switch (chr)
	{
		case '\n':
		case '\r':
			goto newline;
			break;
		case '\t':
			c->x += c->tabwidth - c->x % c->tabwidth;
			break;
		case '\b':
			if (c->x > 0)
			{
				--c->x;
				
				ptr = c->mem + (c->x + c->y * c->width) * 2;
				ptr[0] = 0;
				ptr[1] = c->color;
			}
			break;
		default:
			ptr = c->mem + (c->x + c->y * c->width) * 2;
			ptr[0] = chr;
			ptr[1] = c->color;

			++c->x;
	}

	if (c->x >= c->width)
	{
newline:
		c->x -= c->width;
		if (c->x < 0)
			c->x = 0;
		++c->y;
		if (c->y >= c->height)
		{
			memcpy(c->mem, c->mem + c->width * 2, c->width * (c->height - 1) * 2);
			memset(c->mem + c->width * (c->height - 1) * 2, 0, c->width * 2);
			ptr = c->mem + c->width * (c->height - 1) * 2;
			for (i = 0; i < c->width * 2; i += 2)
			{
				ptr[i] = 0;
				ptr[i + 1] = c->color;
			}
			--c->y;
		}
	}
	unlock();
}

static void key_press(bool special, char value)
{
	if (special)
	{
		switch (value)
		{
			case KEY_LSHIFT:
			case KEY_RSHIFT:
				++shift;
				break;
			case KEY_NUM:
				numlock = !numlock;
				break;
			case KEY_CAPS:
				capslock = !capslock;
				break;
		}
	}
	else
	{
		if (value)
			cons_putchar(cons, value);
	}
}

static void key_release(bool special, char code)
{
	switch (code)
	{
		case KEY_LSHIFT:
		case KEY_RSHIFT:
			--shift;
			break;
	}
}

void kbd_handler(uint index)
{
	u8 	code;
	
	code = kbd_buf[index];
	
	if (code == 0xe0 || code == 0xe1)
	{
		escaped = true;
		return;
	}

	if (code & 0x80)
	{
		if (escaped)
		{
			escaped = false;
			key_release(keymap[code].type, keymap[code].escaped);
		}
		else if (keymap[code & ~0x80].type == 1)
			key_release(true, keymap[code & ~0x80].value);		
	}
	else
	{
		if (escaped)
		{
			escaped = false;
			key_press(keymap[code].type, keymap[code].escaped);
		}
		else if (keymap[code].type == 1)
			key_press(true, keymap[code].value);
		else if (keymap[code].type == 2)
		{
			if (numlock && !shift || !numlock && shift)
				key_press(false, keymap[code].shift);
			else
				key_press(true, keymap[code].value);
		}
		else
		{
			if (shift)
				key_press(false, capslock ? tolower(keymap[code].shift)
						: keymap[code].shift);
			else
				key_press(false, capslock ? toupper(keymap[code].value) 
						: keymap[code].value);
		}
	}
	cons_update(cons);
}

void cons_init()
{
	buffer_register(kbd_handler, &kbd_buf_len);
}

