#ifndef _ISR_H
#define _ISR_H

#include <stdint.h>

#define MAX_INTERRUPTS		256

extern void isr_register_interrupts(uint8_t id, uint32_t exception);
extern void ist_stub_0(void);

#endif
