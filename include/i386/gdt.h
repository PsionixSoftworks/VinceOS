#ifndef _GDT_H
#define _GDT_H

#include <stdint.h>

#define MAX_GDT_ENTRIES		8192
#define MAX_GDT_SIZE		MAX_GDT_ENTRIES * 8

extern void gdt_init(void);

#endif
