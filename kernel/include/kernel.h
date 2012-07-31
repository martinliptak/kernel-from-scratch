#ifndef _KERNEL_H
#define _KERNEL_H

#include "types.h"
#include "stdarg.h"

/*** Common macros and variable declarations that are not defined in 
  any of the source files ***/

#define ARRAY_SIZE(array) sizeof((array)) / sizeof((array)[0])

extern void *_end;
extern void *_start;

/*** printk.c ***/
/*
   Adds the level of verbosity feature to vxprintf and calls it.
   	"<[0-9]> the rest of the format string"
   	Must be at the begining of the format string. 
	The vprintk() prints something only if the "verbosity" command line
	option is equal to or greater than the verbosity specifier in the
	format string.
*/
int vprintk(const char *format, va_list arg);
// More natural version of vprintk. Calls vprintk.
int printk(const char *format, ...);

/*** panic.c ***/
// Reboots the machine
void reboot();
// Disables interrupts and enters an infinite loop.
void hang();
// Prints "Kernel panic: <your formated message>\n" and calls hang().
void panic(const char *format, ...);
// Prints "Kernel warning: <your formated message>\n".
void warn(const char *format, ...);
// Prints "Kernel bug: <your formated message>\n".
void bug(const char *format, ...);

#endif

