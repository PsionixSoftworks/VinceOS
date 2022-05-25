#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <multiboot.h>
#include <driver/vga.h>
#include <kernel.h>

#define CHECK_FLAG(flags, bits)		((flags) & (1 << (bit)))

// Declare global static variables:
unsigned short *vga_mem = NULL;		// Used for displaying ASCII compatible text to the display buffer.
static bool running = true;			// Used to keep the console alive until it's terminated.

void kernel_entry(unsigned long ebx, unsigned long eax)
{
	// Access the video buffer for displaying text on screen. Current mode is 80x25 Color Text Mode.
	vga_mem = (unsigned short *)VGA_ADDR;

	// We need the Multiboot info so we can validate the kernel:
	multiboot_info_t *mb_info = (multiboot_info_t *)ebx;

	// If the ebx register is not Multiboot compliant, this will return and fail.
	if (ebx != MULTIBOOT_BOOTLOADER_MAGIC)	
		return;
	
	// If everything went smoothly, we should see an 'A' and 'B' on the screen:
	OS_SETUP("VinceOS", "0.01u");
	if (info.kernel_name != NULL)
		vga_mem[0] = 'A' | 0xF << 8;
	if (info.kernel_version != NULL)
		vga_mem[1] = 'B' | 0xF << 8;
	
	// This will be used when we start processing input and output:
	while (running)
	{
		// Keep alive!
		;
	}

	// Not technically necessary, but we'll return when we exit the loop:
	return;
}
