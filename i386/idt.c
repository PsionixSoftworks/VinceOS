#include <i386/idt.h>
#include <stdint.h>
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

__attribute__((noreturn))
void exception_handler(void);
void idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags);

void
exception_handler(void)
{
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
		idt_set_descriptor(i, isr_stub_table[i], 0x8E);
		vectors[i] = true;
	}

	__asm__ volatile ("lidt %0" : : "m"(idtr));
	__asm__ volatile ( "sti" );
}
