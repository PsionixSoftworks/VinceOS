#ifndef _IDT_H
#define _IDT_H

#include <isr.h>
#include <stdbool.h>

#define INT_GATE			0x8E
#define USER_INT_GATE		0xEE
#define TRAP_GATE			0x8F

typedef struct 
{
	//uint32_t errcode;
	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
	uint32_t esp;
	uint32_t ss;
} __attribute__((packed)) interrupt_frame_t;

extern void idt_init(void);
extern void idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags);
extern void idt_exception_handle(interrupt_frame_t *frame);
extern void idt_exception_handle_errcode(interrupt_frame_t * frame, uint32_t errcode);
extern bool idt_are_interrupts_enabled(void);
extern void __setup_exceptions(void);


#endif
