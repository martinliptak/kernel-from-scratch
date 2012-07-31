#include "library.h"

void *memcpy(void *dest, const void *src, register unsigned long n)
{
	void *ret = dest;

	if (n == 0)
		return dest;
	while (n--)
		*(u8 *)dest++ = *(u8 *)src++;
	return dest;
}

void *memmove(void *dest, const void *src, register unsigned long n)
{
	void *ret = dest;

	if (n == 0)
		return dest;
	if (src <= dest && src + n > dest)
	{
		dest += n;
		src += n;
		while (n--)
			*(u8 *)--dest = *(u8 *)--src;
	}
	else
		return memcpy(dest, src, n);
	return dest;
}

void *memset(void *s, int c, int n)
{
	void *p = s;

	while (n--)
		*(u8 *)p++ = c;
	return s;
}

int strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && (*s1 == *s2))
	{
		++s1;
		++s2;
	}
	return *s1 - *s2;
}

char *strcpy(char *dest, const char *src)
{
	while (*src)
		*dest++ = *src++;
	*dest = 0;
	return dest;
}

int strlen(const char *s)
{
	int len = 0;

	while (*s++)
		++len;
	return len;
}

