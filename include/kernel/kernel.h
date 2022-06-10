#ifndef _KERNEL_H
#define _KERNEL_H

#include <config.h>

#ifndef _VGA_H
#include "driver/vga.h"
#endif

#include <stdint.h>

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
