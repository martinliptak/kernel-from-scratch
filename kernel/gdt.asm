SECTION .text

	GLOBAL set_gdt, gdt
	
	gdt	dd 0, 0
	code	db 0xff, 0xff, 0, 0, 0, 0x9a, 0xcf, 0
	data	db 0xff, 0xff, 0, 0, 0, 0x92, 0xcf, 0
	times 8192 * 8 - ($ - gdt) db 0
	
	gdtr 	dw 8192 * 8 - 1
		dd gdt

	set_gdt:
		lgdt [gdtr]
		jmp 0x08:.here
		.here:
			mov ax,0x10
			mov ds,ax
			mov es,ax
			mov fs,ax
			mov gs,ax
			mov ss,ax
			ret

