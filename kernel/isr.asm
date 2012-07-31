%macro isr 1
	global isr_%1_wrapper
	extern isr_%1

	isr_%1_wrapper:
		pushad
		push %1
		call [isr_%1]
		add esp, 4
		popad
		iret
%endmacro

SECTION .text
	isr 0
	isr 6
	isr 8
	isr 10
	isr 11
	isr 12
	isr 13
	isr 14
	isr 16
	; isr 32
	isr 33
	
