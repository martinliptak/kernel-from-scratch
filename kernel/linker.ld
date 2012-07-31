__entry__ = 0x100000;
ENTRY(__entry__);

SECTIONS
{
	. = 0xc0100000;
	_start = .;
	.text : AT (ADDR(.text) - 0xc0000000)
	{ 
		*(.text) 
	}
	.data : AT (ADDR(.data) - 0xc0000000)
	{ 
		*(.data) 
		*(.rodata*) 
	}
	.bss : AT (ADDR(.bss) - 0xc0000000)
	{ 
		/* _sbss = .; */
		*(COMMON) 
		*(.bss) 
		/* _ebss = .; */
	}
	_end = .;
}

