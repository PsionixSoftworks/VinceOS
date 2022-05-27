#ifndef _KERNEL_H
#define _KERNEL_H

#ifndef _VERSION_H
#include "version.h"
#endif

#ifndef _VGA_H
#include "driver/vga.h"
#endif

#include <stdint.h>

#define OS_VERSION_MAJOR	0
#define OS_VERSION_MINOR	0
#define OS_VERSION_BUILD	1
#define OS_VERSION_RELEASE	20
#define OS_NAME				"VinceOS"
#define OS_VERSION_STR		"0.01u"
#define OS_VERSION			(OS_VERSION_MAJOR * OS_VERSION_MINOR + OS_VERSION_BUILD * OS_VERSION_RELEASE)

#ifdef __cplusplus
extern "C" {
#endif

enum cursor_type
{
	UNDERLINE,
	BLOCK,
};

void k_putchar(char c);
void k_putstring(char *str);
void k_printf(const char *restrict, ...);

void cursor_enable(enum cursor_type type);
void cursor_disable(void);
void cursor_set_pos(uint8_t x, uint8_t y);
void cursor_update(void);
uint16_t cursor_get_pos(void);

#ifdef __cplusplus
}
#endif

#endif
