#include "sched.h"

uint lock_depth = 0;

void lock()
{
	if (lock_depth == 0)
		irq_off();
	++lock_depth;
}

void unlock()
{
	if (lock_depth == 0)
		return;
	--lock_depth;
	if (lock_depth == 0)
		irq_on();
}

