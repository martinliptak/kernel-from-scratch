#ifndef _LIBRARY_H
#define _LIBRARY_H

#include "types.h"
#include "stdarg.h"

/*** lib/atoi.c ***/
/* 
   Converts a string into an integer. When the number in the string begins 
   with '0' it is converted with base 8 and when it has '0x' prefix the base
   16 is used. Otherwise the base 10 is used.
*/
int atoi(const char *str);
/* 
   Same as atoi(), but returns the number of characters until the first 
   character, that couldn't be used in the conversion. The result is put 
   in the *pn.
*/
int atoin(const char *str, int *pn);

/*** lib/ctype.c ***/ 
int isdigit(char c);
int isblank(char c);
int tolower(char c);
int toupper(char c);

/*** lib/dump.c ***/
// Prints the values of "count" bytes starting with "data" in hexadecimal
void xdump(void *data, int count);
// Same as xdump(), bat in binary
void bdump(void *data, int count);

/*** lib/itoa.c ***/
/*
   Converts "n" to a string pointed to by "buf". If the "base" is 16 the 
   '0x' prefix is prepended. The '0' prefix is prepended when the base is 8.
*/
char *itoa(int n, unsigned int base, char *buf);

/*** lib/sscanf.c ***/
/*
   %% The '%' character
   %i Signed integer
   %s String
   %c Character
   Nothing more is supported.
*/
int vsscanf(const char *str, const char *format, va_list arg);
// More natural version of vsscanf. Calls vsscanf().
int sscanf(const char *str, const char *format, ...);

/*** lib/string.c ***/
void *memmove(void *dest, const void *src, unsigned long n);
void *memcpy(void *dest, const void *src, unsigned long n);
void *memset(void *s, int c, int n);
int strcmp(const char *s1, const char *s2);
char *strcpy(char *dest, const char *src);
int strlen(const char *s);

/*** lib/utoa.c ***/
/*
   Converts "n" to a string pointed to by "buf". If the "base" is 16 the 
   '0x' prefix is prepended. The '0' prefix is prepended when the base is 8.
   In contrast to itoa() it can be used to convert higher pointers. When you
   use itoa() to convert 0xffffffff, it gives you -1. utoa() can deal witch 
   this.
*/
char *utoa(unsigned int n, unsigned int base, char *buf);

/*** lib/dstring.c ***/
typedef struct
{
	char 	*str;
	ulong 	len;
	ulong	allocated;
} dstring_t;
dstring_t *dstring_new(ulong initial_size);
void dstring_free(dstring_t *dstr);
void dstring_append(dstring_t *dstr, const char *str);
void dstring_append_char(dstring_t *dstr, const char c);
int dstring_printf(dstring_t *dstr, const char *format, ...);

/*** lib/vxprintf.c ***/
typedef void (*putchr_t)(void *arg, const char c);
/*
   Calls "putchr" pointer to function to print chars. You can use vxprintf() 
   to print to anything (a string, a console, etc.), when you provide your 
   own "putchr" function. You can also pass one argument to "putchr".
   
   The flag characters
   -	Left justification
   The field width
   	Supported
   The precision
   	NOT supported
   The length modifier
   	NOT supported
   The conversion specifier
   %%	The '%' character
   %s	String
   %c	Character
   %d	Signed integer in decimal
   %o	Signed integer in octal
   %x	Signed integer in hexadecimal
   %u	Unsigned integer in decimal
*/
int vxprintf(putchr_t putchr, void *putchr_arg, const char *format, va_list arg);

#endif
