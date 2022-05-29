#include <pic.h>
#include <portctl.h>

#define PIC_READ_IRR		0x0A
#define PIC_READ_ISR		0x0B

void
pic_send_eoi(uint8_t irq)
{
	if (irq >= 8)
		outb(PIC2_COMMAND, PIC_EOI);
	outb(PIC1_COMMAND, PIC_EOI);
}

void
pic_remap(int offset1, int offset2)
{
	unsigned char a1, a2;

	a1 = inb(PIC1_DATA);
	a2 = inb(PIC2_DATA);

	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	//io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	//io_wait();
	outb(PIC1_DATA, offset1);
	//io_wait();
	outb(PIC2_DATA, offset2);
	//io_wait();
	outb(PIC1_DATA, 4);
	//io_wait();
	outb(PIC2_DATA, 2);
	//io_wait();

	outb(PIC1_DATA, ICW4_8086);
	//io_wait();
	outb(PIC2_DATA, ICW4_8086);
	//io_wait();

	outb(PIC1_DATA, a1);
	outb(PIC2_DATA, a2);
}

void
pic_disable()
{
	__asm__ volatile(
		"mov $0xFF, %al		\n \
		 out %al, $0xa1		\n \
		 out %al, $0x21"
	);
}

void
irq_set_mask(unsigned char irq)
{
	uint16_t port;
	uint8_t value;

	if (irq < 8)
		port = PIC1_DATA;
	else
	{
		port = PIC2_DATA;
		irq -= 8;
	}
	value = inb(port) | (1 << irq);
	outb(port, value);
}

void
irq_clear_mask(unsigned char irq)
{
	uint16_t port;
	uint8_t value;

	if (irq < 8)
		port = PIC1_DATA;
	else
	{
		port = PIC2_DATA;
		irq -= 8;
	}
	value = inb(port) & ~(1 << irq);
	outb(port, value);
}

static uint16_t __pic_get_irq_reg(int ocw3)
{
	outb(PIC1_COMMAND, ocw3);
	outb(PIC2_COMMAND, ocw3);

	return (inb(PIC2_COMMAND) << 8) | inb(PIC1_COMMAND);
}

uint16_t
pic_get_irr(void)
{
	return __pic_get_irq_reg(PIC_READ_IRR);
}

uint16_t
pic_get_isr(void)
{
	return __pic_get_irq_reg(PIC_READ_ISR);
}
