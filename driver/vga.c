#include <driver/vga.h>
#include <stddef.h>

vga_t vga;

/* Initialize the VGA driver. */
void
vga_init(void)
{
	/* The buffer is a pointer to the VGA address. This is located at 0xB8000 on x86 machines. 
	 * The length keeps track of how many characters are in the display buffer. 
	 */
	vga.buffer = (unsigned short *)VGA_ADDR;
	vga.length = 0;
	vga.x = 0;
	vga.y = 0;

	vga_clear_screen();
}

/* Clear the VGA display buffer. */
void
vga_clear_screen(void)
{
	/* We need to iterate through all of the columns first. */
	for (int j = 0; j < VGA_COLS; j++)
	{
		for (int i = 0; i < VGA_ROWS; i++)
		{
			if (vga.buffer != NULL)
			{
				unsigned char color = 0xF | 0x1 << 4;
				vga.buffer[j * VGA_ROWS + i] = ' ' | color << 8;
				vga.length = 0;
				vga.x = 0;
				vga.y = 0;
			}
		}
	}
}
