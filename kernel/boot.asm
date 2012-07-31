%define STACK_SIZE 	0x4000
%define MAGIC 		0x1BADB002
%define FLAGS 		3
%define VIRTUAL_BASE 	0xc0000000
%define PAGE_NUMBER	(VIRTUAL_BASE >> 22)
	
section .text
	global pagedir
	extern kmain

	main:	
		mov ecx, pagedir - VIRTUAL_BASE ; We need physical address
		and ecx, 0xfffff000		; The last 3 bits are flags
		mov cr3, ecx

		mov ecx, cr4
		or ecx, 0x00000010		; PSE (4MB pages)
		mov cr4, ecx
			
		mov ecx, cr0
		or ecx, 0x80000000		; PG (paging)
		mov cr0, ecx

		lea ecx, [higher_main]
		jmp ecx
		
	higher_main:
		; We don't need 1:1 mapping of the first 4MB, only 1MB, 
		; except the first page (to detect NULL pointers)
		; xor ecx, ecx
		; inc ecx
		; .loop:
		;	mov edx, ecx
		;	shl edx, 12			; edx *= 4096
		;	or edx, 0x3			; flags present and read/write
		;	mov [firsttable + ecx * 4], edx
		;	inc ecx
		;	cmp ecx, 256
		;	jb .loop
		; mov dword [pagedir], (firsttable - $$ + 0x100000) & 0xfffff000 | 0x00000003
		mov dword [pagedir], 0
		invlpg [0]

		mov ecx, pagedir - VIRTUAL_BASE
		or ecx, 0x3
		mov [pagedir + 4 * 1022], ecx
		invlpg [0xfffff000]

		mov esp, stack + STACK_SIZE

		push ebx
		call kmain
		jmp $

	align 4
	header:
		dd MAGIC
		dd FLAGS
		dd -(MAGIC + FLAGS)
		
	align 4096
	pagedir:
		; Without this entry the kernel would crash immediately after enabling 
		; paging, because the cpu wouldn't be able to fetch the next instruction.
		dd 0x00000083
		times (PAGE_NUMBER - 1) dd 0
		; Map the first 16MB to the beginning of the kernel virtual base
		dd 0x00000083
		dd 0x00400083
		dd 0x00800083
		dd 0x00c00083
		times (1024 - PAGE_NUMBER - 4) dd 0
	
	align 4096
	firsttable:
		times 1024 dd 0

section .bss
	align 32
	stack:
		resb STACK_SIZE

