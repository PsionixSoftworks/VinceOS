#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <multiboot.h>
#include <driver/vga.h>
#include <kernel.h>
#include <i386/gdt.h>
#include <i386/idt.h>

#include <pic.h>
#include <isr.h>
#include <boot.h>

#define CHECK_FLAG(flags, bits)		((flags) & (1 << (bits)))

// Declare global static variables:
static bool running = true;			// Used to keep the console alive until it's terminated.

void kernel_entry(unsigned long ebx, unsigned long eax)
{
	// Initialize the VGA driver and clear the screen:
	vga_init();
	cursor_enable(BLOCK);

	// Setup the GDT and the IDT:
	gdt_init();
	idt_init();

	pic_disable();
	pic_remap(0x20, 0x28);

	__asm__ volatile ( "sti" );

	__setup_exceptions();

	bool ints_enabled = idt_are_interrupts_enabled();
	if (ints_enabled)
		k_printf("Interrupts are enabled...\n");

	// We need the Multiboot info so we can validate the kernel:
	multiboot_info_t *mb_info = (multiboot_info_t *)eax;

	// If the ebx register is not Multiboot compliant, this will return and fail.
	if (ebx != MULTIBOOT_BOOTLOADER_MAGIC)	
	{
		k_printf("[ERROR]: %s\n", "Invalid Magic Number: 0x%x");
		return;
	}

	// If everything went smoothly, we should see an the kernel name and version on the screen:
	OS_SETUP(OS_NAME, OS_VERSION);
	if (info.kernel_name != NULL)
		k_printf("Kernel Name: %s\n", info.kernel_name);
	if (info.kernel_version > 0)
		k_printf("Kernel Version: %d\n", info.kernel_version);

	// Now we check the value of the Multiboot Info flags:
	k_printf("Multiboot Info Flags: 0x%x\n", (unsigned)mb_info->flags);	// 0x1A67? What does this do?

	// Get the memory lower and upper values:
	if (CHECK_FLAG(mb_info->flags, 0))
		k_printf("Memory Lower: %uKB\nMemory Upper: %uKB\n", 
			(unsigned)mb_info->mem_lower,
			(unsigned)mb_info->mem_upper
		);
	
	// Check that the boot device is valid:
	if (CHECK_FLAG(mb_info->flags, 1))
		k_printf("Boot Device: 0x%x\n", (unsigned)mb_info->boot_device);
	
	// Check if the command line has passed:
	if (CHECK_FLAG(mb_info->flags, 2))
		k_printf("Command Line: %s\n", mb_info->cmdline);

	// Check if mods are valid:
	if (CHECK_FLAG(mb_info->flags, 3))
	{
		multiboot_module_t *mod;
		unsigned int i;

		k_printf("Mods Count: %d\nMods Addr: 0x%x\n",
			(int)mb_info->mods_count, 
			(int)mb_info->mods_addr
		);
		for (i = 0, mod = (multiboot_module_t *)mb_info->mods_addr; i < mb_info->mods_count; i++, mod++)
		{
			k_printf("Mod Start: 0x%x\nMod End: 0x%x\nCmd Line: %s\n",
				(unsigned)mod->mod_start,
				(unsigned)mod->mod_end,
				(unsigned)mod->cmdline
			);
		}
	}

	if (CHECK_FLAG(mb_info->flags, 4) && CHECK_FLAG(mb_info->flags, 5))
	{
		k_printf("Bits 4 and 5 have been set.\n");
		return;
	}

	/* Check if a.out is supported */
	if (CHECK_FLAG(mb_info->flags, 4))
	{
		multiboot_aout_symbol_table_t *multiboot_aout_sym = &(mb_info->u.aout_sym);
		k_printf("Multiboot AOUT Symbol Table:\nTable Size: 0x%x, String Size: 0x%x, Address: 0x%x\n",
			(unsigned)multiboot_aout_sym->tabsize,
			(unsigned)multiboot_aout_sym->strsize,
			(unsigned)multiboot_aout_sym->addr
		);
	}

	if (!USE_GFX_MODE)
		k_printf("Using Color Text mode...\n");

	// This will be used when we start processing input and output:
	while (running)
	{
		// Keep alive!
		;
	}

	// Not technically necessary, but we'll return when we exit the loop:
	return;
}
