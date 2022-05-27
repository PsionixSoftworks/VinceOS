#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <multiboot.h>
#include <driver/vga.h>
#include <kernel.h>
#include <i386/gdt.h>

#define CHECK_FLAG(flags, bits)		((flags) & (1 << (bit)))

// Declare global static variables:
static bool running = true;			// Used to keep the console alive until it's terminated.

void kernel_entry(unsigned long ebx, unsigned long eax)
{
	// Initialize the VGA driver and clear the screen:
	vga_init();
	cursor_enable(BLOCK);

	gdt_init();

	// We need the Multiboot info so we can validate the kernel:
	multiboot_info_t *mb_info = (multiboot_info_t *)ebx;

	// If the ebx register is not Multiboot compliant, this will return and fail.
	if (ebx != MULTIBOOT_BOOTLOADER_MAGIC)	
		return;
	
	// If everything went smoothly, we should see an 'A' and 'B' on the screen:
	OS_SETUP(OS_NAME, OS_VERSION);
	if (info.kernel_name != NULL)
		k_printf("Kernel Name: %s\n", info.kernel_name);
	if (info.kernel_version > 0)
		k_printf("Kernel Version: %d\n", info.kernel_version);
	
	
	// This will be used when we start processing input and output:
	while (running)
	{
		// Keep alive!
		;
	}

	// Not technically necessary, but we'll return when we exit the loop:
	return;
}
