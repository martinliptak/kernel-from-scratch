#ifndef _MULTIBOOT_H
#define _MULTIBOOT_H

#include "types.h"

typedef struct elf_section_header_table
{
	u32 num;
	u32 size;
	u32 addr;
	u32 shndx;
} elf_section_header_table_t;

typedef struct multiboot_info
{
	u32 		flags;
	u32 		mem_lower;
	u32 		mem_upper;
	u32 		boot_device;
	const char 	*cmdline;
	u32 		mods_count;
	u32 		mods_addr;
	elf_section_header_table_t elf_sec;
	u32 		mmap_length;
	u32 		mmap_addr;
} multiboot_info_t;

/*** main.c ***/

extern struct multiboot_info *mb_info;

#endif
