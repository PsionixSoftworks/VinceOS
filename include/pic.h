#ifndef _PIC_H
#define _PIC_H

#include <stdint.h>

#define PIC1			0x20
#define PIC2			0xA0
#define PIC1_COMMAND	PIC1
#define PIC1_DATA		(PIC1 + 1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA		(PIC2 + 1)
#define PIC_EOI			0x20

#define ICW1_ICW4		0x01
#define ICW1_SINGLE		0x02
#define ICW1_INTERVAL4	0x04
#define ICW1_LEVEL		0x08
#define ICW1_INIT		0x10

#define ICW4_8086		0x01
#define ICW4_AUTO		0x02
#define ICW4_BUF_SLAVE	0x08
#define ICW4_BUF_MASTER	0x0C
#define ICW4_SFNM		0x10

extern void pic_send_eoi(uint8_t irq);
extern void pic_remap(int offset1, int offset2);
extern void pic_disable(void);
extern void irq_set_mask(unsigned char irq);
extern void irq_clear_mask(unsigned char irq);
extern uint16_t pic_get_irr(void);
extern uint16_t pic_get_isr(void);

#endif
