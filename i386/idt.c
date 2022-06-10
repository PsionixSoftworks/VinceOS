#include <i386/idt.h>
#include <stdint.h>
#include <pic.h>
#include <kernel/kernel.h>
#include <exception.h>
#include <stdbool.h>

typedef struct
{
	uint16_t isr_low;
	uint16_t kernel_cs;
	uint8_t reserved;
	uint8_t attributes;
	uint16_t isr_high;
} __attribute__((packed)) idt_entry_t;

typedef struct
{
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) idtr_t;

__attribute__((aligned(16)))
static idt_entry_t 	idt[MAX_INTERRUPTS];
static idtr_t 		idtr;
static bool 		vectors[32];

extern void *isr_stub_table[];

void exception_handler(void);
void idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags);

static inline bool __is_errcode(uint8_t i);

void
exception_handler(void)
{
	k_printf("Exception raised!!!\n");
	__asm__ volatile( "cli; hlt" );
}

void
idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags)
{
	idt_entry_t *descriptor = &idt[vector];

	descriptor->isr_low		= (uint32_t)isr & 0xFFFF;
	descriptor->kernel_cs	= 0x08;
	descriptor->attributes	= flags;
	descriptor->isr_high	= (uint32_t)isr >> 16;
	descriptor->reserved	= 0;
}

void
idt_init(void)
{
	idtr.base = (uintptr_t)&idt[0];
	idtr.limit = (uint16_t)sizeof(idt_entry_t) * MAX_INTERRUPTS - 1;

	for (uint8_t i = 0; i < 32; i++)
	{
		if (__is_errcode(i))
			idt_set_descriptor(i, idt_exception_handle_errcode, TRAP_GATE);
		else
			idt_set_descriptor(i, idt_exception_handle, INT_GATE);
		vectors[i] = true;
	}

	__asm__ volatile ("lidt %0" : : "m"(idtr));	
}

bool
idt_are_interrupts_enabled(void)
{
	unsigned long flags;
	__asm__ volatile (
		"pushf	\n\t"
		"pop %0"
		: "=g"(flags)
	);
	return flags & (1 << 9);
}

void
idt_exception_handle(interrupt_frame_t *frame)
{
	k_printf("Exception Handled: No description available! No Errcode...\n");
}

void
idt_exception_handle_errcode(interrupt_frame_t *frame, uint32_t errcode)
{
	k_printf("Exception Handled: No description available! Errcode: 0x%x\n", errcode);
}

static inline bool
__is_errcode(uint8_t i)
{
	return (
		(i == 8) 	||
		(i == 10)	||
		(i == 11)	||
		(i == 12)	||
		(i == 13)	||
		(i == 14)	||
		(i == 17)	||
		(i == 21)
	);
}

void
__setup_exceptions(void)
{
	idt_set_descriptor(0, divide_by_zero_except, 			TRAP_GATE);
	idt_set_descriptor(1, debug_except, 					TRAP_GATE);
	idt_set_descriptor(2, nmi_except, 						TRAP_GATE);
	idt_set_descriptor(3, breakpoint_except,				TRAP_GATE);
	idt_set_descriptor(4, overflow_except, 					TRAP_GATE);
	idt_set_descriptor(5, bounds_check_except,				TRAP_GATE);
	idt_set_descriptor(6, invalid_opcode_except,			TRAP_GATE);
	idt_set_descriptor(7, no_coprocessor_except,			TRAP_GATE);
	idt_set_descriptor(8, double_fault_except,				INT_GATE);
	idt_set_descriptor(9, coproc_seg_overrun_except, 		TRAP_GATE);
	idt_set_descriptor(10, invalid_tss_except, 				INT_GATE);
	idt_set_descriptor(11, no_segment_except,				INT_GATE);
	idt_set_descriptor(12, stack_segment_overrun_except, 	INT_GATE);
	idt_set_descriptor(13, gpf_except, 						INT_GATE);
	idt_set_descriptor(14, page_fault_except, 				INT_GATE);
	idt_set_descriptor(15, unassigned_except,				TRAP_GATE);
	idt_set_descriptor(16, coproc_except, 					TRAP_GATE);
	idt_set_descriptor(17, alignment_check_except, 			INT_GATE);
	idt_set_descriptor(18, machine_check_except,			TRAP_GATE);
}
