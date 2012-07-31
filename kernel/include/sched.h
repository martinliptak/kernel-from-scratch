#ifndef _PROCESS_H
#define _PROCESS_H

#include "types.h"

typedef struct process
{
	/* Never change the order of the first 3 memebers. They are 
	   used in asm code in timer.asm. */
	ulong esp;
	ulong ss;
	ulong cr3;
} process_t;

typedef struct process_queue
{
	process_t *proc;
	struct process_queue *next;
} process_queue_t;

typedef struct {
	ushort backlink, __blh;
	uint   esp0;
	ushort ss0, __ss0h;
	uint   esp1;
	ushort ss1, __ss1h;
	uint   esp2;
	ushort ss2, __ss2h;
	uint   cr3;
	uint   eip;
	uint   eflags;
	uint   eax, ecx, edx, ebx;
	uint   esp, ebp, esi, edi;
	ushort es, __esh;
	ushort cs, __csh;
	ushort ss, __ssh;
	ushort ds, __dsh;
	ushort fs, __fsh;
	ushort gs, __gsh;
	ushort ldt, __ldth;
	ushort trace, bitmap;
} tss_t;

extern process_t *current;
extern process_queue_t *run_queue;

#endif

