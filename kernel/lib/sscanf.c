#include "stdarg.h"

int vsscanf(const char *str, const char *format, va_list arg)
{
	const char 	*f;
	const char 	*s;
	int		count = 0;
	int		len;
	char		*c;

	for (f = format, s = str; *f, *s; ++f)
	{
		if (*f == '%')
		{
			switch (*++f)
			{
			case '%':
				goto noconv;
				break;
			case 'i':
				s += len = atoin(s, va_arg(arg, int));
				if (len)
					++count;
				break;
			case 's':
				for (c = va_arg(arg, char *); *s && !isblank(*s); ++s, ++c)
					*c = *s;
				*c = '\0';
				++count;
				break;
			case 'c':
				*va_arg(arg, char *) = *s++;
				++count;
				break;
			default:
				bug("Invalid conversion specifier: %s", format);
			}
		}
		else
		{
noconv:
			if (*s != *f)
				return count;
			++s;
		}
	}
		
	return count;
}

int sscanf(const char *str, const char *format, ...)
{
	va_list	arg;
	int	ret;

	va_start(arg, format);
	ret = vsscanf(str, format, arg);
	va_end(arg);

	return ret;
}

