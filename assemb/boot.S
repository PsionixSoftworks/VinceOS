#ifndef _BOOTLOADER_S
#define _BOOTLOADER_S	1

#define ASM_FILE		1
#include <multiboot.h>
#include <boot.h>

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
1: 	hlt
	jmp 1b
.comm stack, STACK_SIZE

#endif
