#include "library.h"
#include "console.h"

extern int opt_verbosity;

static void putchr(void *arg, const char c)
{
	cons_putchar(cons, c);
}

int vprintk(const char *format, va_list arg)
{
	int ret;
	
	if (format[0] == '<')
	{
		if (format[1] - '0' > opt_verbosity)
			return 0;
		format += 3;
	}
	
	ret = vxprintf(putchr, NULL, format, arg);
	cons_update(cons);
	
	return ret;
}

int printk(const char *format, ...)
{
	va_list	arg;
	int	ret;

	if (format[0] == '<')
	{
		if (format[1] - '0' > opt_verbosity)
			return 0;
		format += 3;
	}
	
	va_start(arg, format);
	ret = vxprintf(putchr, NULL, format, arg);
	va_end(arg);
	cons_update(cons);

	return ret;
}

