#include "library.h"

static void spaces(putchr_t putchr, void *putchr_arg, int n)
{
	// When the string is longer than alignment n is negative. It should not print
	// any spaces.
	while (n-- > 0)
		putchr(putchr_arg, ' ');
}

static void putstr(putchr_t putchr, void *putchr_arg, const char *str)
{
	while (*str)
		putchr(putchr_arg, *str++);
}

int vxprintf(putchr_t putchr, void *putchr_arg, const char *format, va_list arg)
{
	const char	*f;
	int		count = 0;

	char		buf[80];
	int		len;
	const char	*str;
	int		align;
	
	for (f = format; *f; ++f)
	{
		if (*f == '%')
		{
			++f;
			align = 0;
conversion:
			switch (*f)
			{
				case '%':
					buf[0] = '%';
					buf[1] = NULL;
					len = 1;
					str = buf;
					break;
				case 's':
					str = va_arg(arg, char *);
					len = strlen(str);
					break;
				case 'c':
					buf[0] = va_arg(arg, int);
					buf[1] = NULL;
					len = 1;
					str = buf;
					break;
				case 'd':
					itoa(va_arg(arg, int), 10, buf);
					len = strlen(buf);
					str = buf;
					break;
				case 'o':
					utoa(va_arg(arg, unsigned int), 8, buf);
					len = strlen(buf);
					str = buf;
					break;
				case 'u':
					utoa(va_arg(arg, unsigned int), 10, buf);
					len = strlen(buf);
					str = buf;
					break;
				case 'x':
					utoa(va_arg(arg, unsigned int), 16, buf);
					len = strlen(buf);
					str = buf;
					break;
					break;
				case 'b':
					utoa(va_arg(arg, unsigned int), 2, buf);
					len = strlen(buf);
					str = buf;
					break;
				default:
					if (isdigit(*f) || *f == '-')
					{
						f += atoin(f, &align);
						goto conversion;
					}
					else
						bug("Invalid conversion specifier \'%c\' "
								"in \'%s\'", *f, format);
			}

			if (align < 0)
			{
				putstr(putchr, putchr_arg, str);
				spaces(putchr, putchr_arg, -align - len);
			}
			else
			{
				spaces(putchr, putchr_arg, align - len);
				putstr(putchr, putchr_arg, str);
			}
			align = align < 0 ? -align : align;
			len = len < align ? align : len;
		}
		else
		{
			putchr(putchr_arg, *f);
			len = 1;
		}
		count += len;
	}

	va_end(arg);
	return count;
}

