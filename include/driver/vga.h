#ifndef _VGA_H
#define _VGA_H

#define VGA_ROWS			80
#define VGA_COLS			25
#define VGA_ATTR			0x7
#define VGA_ADDR			0x000B8000

typedef struct vga
{
	unsigned short *buffer;
	unsigned int length;
	unsigned char x;
	unsigned char y;
} vga_t;

extern void vga_init(void);
extern void vga_clear_screen(void);

#endif
