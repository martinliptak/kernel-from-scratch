SECTION .text
	global isr_32_wrapper
	extern run_queue

	isr_32_wrapper:
		; Push registers to the current process stack
		pushad
		; ++timer_ticks
		mov eax, [timer_ticks]
		inc eax
		mov [timer_ticks], eax
		; run_queue->proc->ss = %ss; run_queue->proc->esp = %esp; 
		; run_queue->proc->cr3 = %cr3
		; See sched.c and process_t in include/sched.h
		mov eax, [run_queue]
		mov ebx, [eax]
		mov eax, ebx
		mov [eax], esp
		mov [eax + 4], ss
		mov ebx, cr3
		mov [eax + 8], ebx
		; run_queue = run_queue->next
		mov eax, [run_queue]
		mov ebx, [eax + 4]
		mov [run_queue], ebx
		; Switch to the stack of the next process 
		mov eax, [ebx]
		mov ebx, eax
		mov ax, [ebx + 4]
		mov ss, ax
		mov esp, [ebx]
		mov eax, [ebx + 8]
		mov cr3, eax
		; Tell the PIC to raise the next pending interrupt
		mov al, 0x20
		out 0x20, al
		; Pop the registers that were pushed a few ticks ago
		popad
		; cs, eip and eflags are on the stack
		iret

SECTION .data
	global timer_ticks
	
	timer_ticks dd 0

