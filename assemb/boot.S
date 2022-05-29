#ifndef _BOOTLOADER_S
#define _BOOTLOADER_S	1

#define ASM_FILE		1
#include <multiboot.h>

/* Set the macro below to 1 to enable Graphic Mode */
#define USE_GFX_MODE	0

/* For consistency sake, take off the 'MULTIBOOT_' prefix so 'AOUT_KLUDGE' isn't alone. */
#define HEADER_MAGIC	MULTIBOOT_HEADER_MAGIC
#define PAGE_ALIGN		MULTIBOOT_PAGE_ALIGN
#define MEMORY_INFO		MULTIBOOT_MEMORY_INFO

/* We want to be able to just toggle Graphic Mode on and off, so we'll de this */
#if defined(USE_GFX_MODE) && USE_GFX_MODE == 1
#define VIDEO_MODE		MULTIBOOT_VIDEO_MODE
#else
#define VIDEO_MODE		0
#endif

/* Check if the '__ELF__' macro is defined or not. If not, it will expand to the AOUT Kludge. */
#ifdef __ELF__
#define AOUT_KLUDGE		0
#else
#define AOUT_KLUDGE		MULTIBOOT_AOUT_KLUDGE
#endif

#define HEADER_FLAGS	(PAGE_ALIGN | MEMORY_INFO |  VIDEO_MODE | AOUT_KLUDGE)
#define STACK_SIZE		(1 << 16)

/* Check if this is an assembler file (hint, it is. Duh!) */
#ifdef __ASSEMBLER__
/* Define the entry point for the linker */
.globl _start

/* Locate the kernel entry externally */
.extern kernel_entry

/* Start using multiboot header */
.section .text

/* Jump to the multiboot entry code */
_start:
	jmp multiboot_entry
	.align 4

multiboot_header:
	.long HEADER_MAGIC
	.long HEADER_FLAGS
	.long -(HEADER_MAGIC + HEADER_FLAGS)

/* Now we need to set the addresses of the header, loader, bss, and entry. */
#ifdef __ELF__
.long 0						/* Multiboot Header */
.long 0						/* Load Address */
.long 0						/* Load End Address */
.long 0						/* BSS End Address */
.long 0						/* Entry Address */
#else
.long multiboot_header		/* Multiboot Header */
.long _start				/* Load Address */
.long _edata				/* Load End Address */
.long _end					/* BSS End Address */
.long multiboot_entry		/* Entry Address */
#endif

/* Now, if we're in Graphic Mode, we need to setup the resolution and the bits per pixel */
.long 0						
.long 1280					/* Width resolution */
.long 720					/* Height resolution */
.long 32					/* Pixel depth (bits per pixel, or BPP) */

/* Setup the multiboot entry as called by the '_start' label */
multiboot_entry:
	/* Initialize the stack pointer */
	movl $(stack + STACK_SIZE), %esp

	/*Reset the EFLAGS */
	pushl $0
	popf

	/* These two are called from 'kernel_entry.' EBX represents the Multiboot info struct, and EAX represents the magic number, provided all went well */
	pushl %ebx
	pushl %eax

	/* Now we need to call the kernel entry and run the halt loop */
	call kernel_entry
	cli
1: 	hlt
	jmp 1b
.comm stack, STACK_SIZE
#endif

#endif
