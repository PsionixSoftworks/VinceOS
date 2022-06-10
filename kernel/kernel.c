#include <kernel/kernel.h>
#include <portctl.h>

#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

#define CURSOR_CMD		0x3D4
#define CURSOR_DATA		0x3D5

extern vga_t vga;

void
k_putchar(char c)
{
	unsigned char color = 0xF | 0x1 << 4;

	if (c == '\n' || vga.x > VGA_ROWS)
	{
		vga.x = 0;
		vga.y++;
	}
	else
	{
		vga.buffer[vga.y * VGA_ROWS + vga.x] = c | color << 8;
		vga.length++;
		vga.x++;
	}
	cursor_update();
}

void
k_putstring(char *str)
{
	for (size_t i = 0; i < strlen(str); ++i)
		k_putchar(str[i]);
}

void
k_printf(const char *restrict format, ...)
{
	va_list ap;
	va_start(ap, format);
	for (size_t i = 0; i < strlen(format); ++i)
	{
		if (format[i] == '%')
		{
			switch (format[i + 1])
			{
				case 's': {								// For strings
					char *__input = va_arg(ap, char *);
					k_putstring(__input);
					i++;
					continue;
				}
				case 'b': {								// For binary
					uint32_t __input = va_arg(ap, uint32_t);
					char buffer[16];
					k_putstring(itoa(__input, buffer, 2));
					i++;
					continue;
				}
				case 'c': {								// For char
					char *__input = va_arg(ap, char *);
					k_putchar((int)__input);
					i++;
					continue;
				}
				case 'd': {								// For decimal (numbers; only supprts int for now)
					int __input = va_arg(ap, int);
					char buffer[16];
					k_putstring(itoa(__input, buffer, 10));
					i++;
					continue;
				}
				case 'u': {
					unsigned int __input = va_arg(ap, unsigned int);
					char buffer[256];
					k_putstring(itoa(__input, buffer, 10));
					i++;
					continue;
				}
				case 'o': {								// For octal
					long __input = va_arg(ap, long);
					char buffer[16];
					k_putstring(itoa(__input, buffer, 8));
					i++;
					continue;
				}
				case 'x': {								// For hex (lowecase)
					unsigned long __input = va_arg(ap, unsigned long);
					char buffer[16];
					k_putstring(itoa(__input, buffer, 16));
					i++;
					continue;
				}
			};
		}
		else
		{
			k_putchar(format[i]);
		}
	}
	va_end(ap);
}

void
cursor_enable(enum cursor_type type)
{
	switch (type)
	{
		case UNDERLINE: {
			uint8_t start 	= (inb(CURSOR_DATA) & 0xC0) | 0x0E;
			uint8_t end 	= (inb(CURSOR_DATA) & 0xE0) | 0x0F;
			outb(CURSOR_CMD, 0x0A);
			outb(CURSOR_DATA, start);

			outb(CURSOR_CMD, 0x0B);
			outb(CURSOR_DATA, end);
		};
		break;
		case BLOCK: {
			uint8_t start 	= (inb(CURSOR_DATA) & 0xC0) | 0x00;
			uint8_t end 	= (inb(CURSOR_DATA) & 0xE0) | 0x0F;
			outb(CURSOR_CMD, 0x0A);
			outb(CURSOR_DATA, start);

			outb(CURSOR_CMD, 0x0B);
			outb(CURSOR_DATA, end);
		};
		break;
	}
}

void
cursor_disable(void)
{
	outb(CURSOR_CMD, 0x0A);
	outb(CURSOR_DATA, 0x20);
}

void
cursor_set_pos(uint8_t x, uint8_t y)
{
	uint16_t pos = y * VGA_ROWS + x;

	outb(CURSOR_CMD, 0x0F);
	outb(CURSOR_DATA, (uint8_t)(pos & 0xFF));
	outb(CURSOR_CMD, 0x0E);
	outb(CURSOR_DATA, (uint8_t)((pos >> 8) & 0xFF));
}

void
cursor_update(void)
{
	cursor_set_pos(vga.x, vga.y);
}

uint16_t
cursor_get_pos(void)
{
	uint16_t pos = 0;
	outb(CURSOR_CMD, 0x0F);
	pos |= inb(CURSOR_DATA);
	outb(CURSOR_CMD, 0x0E);
	pos |= ((uint16_t)inb(CURSOR_DATA)) << 8;

	return pos;
}
