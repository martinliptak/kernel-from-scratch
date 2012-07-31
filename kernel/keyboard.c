#include "keyboard.h"
#include "console.h"
#include "interrupt.h"

u8 	kbd_buf[256];
uint 	kbd_buf_len = 0;

void isr_21h()
{
	if (kbd_buf_len > 255)
	{
		warn("Keyboard buffer full");
		inb(0x60);
	}
	else
	{
		kbd_buf[kbd_buf_len++] = inb(0x60);
	}
	pic_unlock(false);
}

void kbd_init()
{
	isr_33 = isr_21h;
	pic_unmask(1);
}

