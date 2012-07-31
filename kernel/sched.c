#include "sched.h"
#include "memory.h"
#include "kernel.h"

process_queue_t *run_queue = NULL;
process_t	*current;

void sched_init()
{
	run_queue = malloc(sizeof(process_queue_t));
	
	run_queue[0].proc = malloc(sizeof(process_t));
	memset(run_queue[0].proc, 0, sizeof(run_queue[0].proc));
	run_queue[0].next = run_queue;
	
	current = run_queue->proc;
	
	pic_unmask(0);
}

