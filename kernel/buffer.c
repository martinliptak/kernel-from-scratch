#include "buffer.h"
#include "kernel.h"

buffer_t buffer[32] = {};

unsigned int buffer_register(buffer_handler_t handler, uint *len)
{
	int i;

	lock();
	for (i = 0; i < ARRAY_SIZE(buffer); ++i)
	{
		if (buffer[i].handler == NULL)
		{
			buffer[i].handler = handler;
			buffer[i].len = len;

			unlock();
			return i;
		}
	}
	unlock();
	bug("Buffer table full");
	return 0;
}

void buffer_call_handlers()
{
	int i;

	for (i = 0; i < ARRAY_SIZE(buffer); ++i)
	{
		if (buffer[i].len && *buffer[i].len)
		{
			buffer[i].handler(--(*buffer[i].len));
		}
	}
}

