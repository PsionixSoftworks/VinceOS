#ifndef _IDT_H
#define _IDT_H

#include <stdbool.h>

#define MAX_INTERRUPTS		256

void idt_init(void);
bool are_interrupts_enabled(void);

#endif
