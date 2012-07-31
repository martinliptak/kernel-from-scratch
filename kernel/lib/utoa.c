#include "types.h"

char *utoa(unsigned int n, unsigned int base, char *buf)
{
	int 	i = 0;
	int 	j;
	char	tmp;

	if (base == 16)
	{
		*buf++ = '0';
		*buf++ = 'x';
	}
	else if (base == 8)
		*buf++ = '0';
	
	do
	{
		buf[i] = n % base + '0';
		if (buf[i] > '9')
			buf[i] += 39;
		n /= base;
		++i;
	}
	while (n);
	buf[i--] = NULL;

	for (j = 0; j < i; --i, ++j)
	{
		tmp = buf[j];
		buf[j] = buf[i];
		buf[i] = tmp;
	}

	return buf;
}

