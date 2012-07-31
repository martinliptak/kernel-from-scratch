SECTION .text
	GLOBAL set_idt, idt

	idt	times 256 dd 0, 0

	idtr	dw 256 * 8 - 1
		dd idt
	
	set_idt:
		lidt [idtr]
		ret
	
