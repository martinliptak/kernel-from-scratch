#include "library.h"
#include "memory.h"

dstring_t *dstring_new(ulong initial_size)
{
	dstring_t *dstr;

	dstr = malloc(sizeof(dstring_t));
	if (dstr == NULL)
		return NULL;
	
	dstr->str = malloc(initial_size);
	if (dstr->str == NULL)
	{
		free(dstr);
		return NULL;
	}
	dstr->len = 0;
	dstr->allocated = initial_size;
	dstr->str[0] = 0;
	
	return dstr;
}

void dstring_free(dstring_t *dstr)
{
	free(dstr->str);
	free(dstr);
}

void dstring_append(dstring_t *dstr, const char *str)
{
	int i;

	for (i = 0; str[i]; ++i)
	{
		if (dstr->len >= dstr->allocated)
		{
			dstr->allocated += 16;
			dstr->str = realloc(dstr->str, dstr->allocated);
		}
		dstr->str[dstr->len] = str[i];
		++dstr->len;
	}
	dstr->str[dstr->len] = 0;
}

void dstring_append_char(dstring_t *dstr, const char c)
{
	if (dstr->len >= dstr->allocated)
	{
		dstr->allocated += 16;
		dstr->str = realloc(dstr->str, dstr->allocated);
	}
	dstr->str[dstr->len] = c;
	++dstr->len;
	dstr->str[dstr->len] = 0;
}

int dstring_printf(dstring_t *dstr, const char *format, ...)
{
	va_list arg;
	int	ret;

	va_start(arg, format);
	ret = vxprintf((putchr_t)dstring_append_char, (void *)dstr, format, arg);
	va_end(arg);

	return ret;
}

