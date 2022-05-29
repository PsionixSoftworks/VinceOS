#ifndef _PORTCTL_H
#define _PORTCTL_H

#define outb(port, value)		__outb(port, value)
#define outw(port, value)		__outw(port, value)
#define outl(port, value)		__outl(port, value)

#define inb(port)				__inb(port)
#define inw(port)				__inw(port)
#define inl(port)				__inl(port)

#define io_wait()				__io_wait()

static inline void 
__outb(uint16_t port, uint8_t value)
{
	__asm__ volatile( "outb %0, %1" 
		: : "a"(value), "Nd"(port) );
}

static inline void 
__outw(uint16_t port, uint16_t value)
{
	__asm__ volatile( "outw %0, %1" 
		: : "a"(value), "Nd"(port) );
}

static inline void 
__outl(uint16_t port, uint32_t value)
{
	__asm__ volatile( "outl %0, %1" 
		: : "a"(value), "Nd"(port) );
}

static inline uint8_t 
__inb(uint16_t port)
{
	uint8_t value;
	__asm__ volatile( "inb %1, %0" 
		: "=a"(value) 
		: "Nd"(port) );
	return (value);
}

static inline uint16_t 
__inw(uint16_t port)
{
	uint16_t value;
	__asm__ volatile( "inb %1, %0" 
		: "=a"(value) 
		: "Nd"(port) );
	return (value);
}

static inline uint32_t 
__inl(uint16_t port)
{
	uint32_t value;
	__asm__ volatile( "inb %1, %0" 
		: "=a"(value) 
		: "Nd"(port) );
	return (value);
}

static inline void
__io_wait(void)
{
	__outb(0x80, 0);
}

#endif
