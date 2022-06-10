#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include <i386/idt.h>
#include <kernel/kernel.h>

/* INT $0x0 */
__attribute__((interrupt)) static inline void
divide_by_zero_except(interrupt_frame_t *frame __attribute__((unused)))
{
	k_printf("Exception Raised: Divide by zero.\n");
	return;
}

/* INT $0x1 */
__attribute__((interrupt)) static inline void
debug_except(interrupt_frame_t *frame __attribute__((unused)))
{
	k_printf("Exception Raised: Debug Exception.\n");
	return;
}

/* INT $0x2 */
__attribute__((interrupt)) static inline void
nmi_except(interrupt_frame_t *frame __attribute__((unused)))
{
	k_printf("Exception Raised: Non Maskable Interrupt Exception.\n");
	return;
}

/* INT $0x3 */
__attribute__((interrupt)) static inline void
breakpoint_except(interrupt_frame_t *frame __attribute__((unused)))
{
	k_printf("Exception Raised: Breakpoint Exception.\n");
	return;
}

/* INT $0x4 */
__attribute__((interrupt)) static inline void
overflow_except(interrupt_frame_t *frame __attribute__((unused)))
{
	k_printf("Exception Raised: Overflow Exception.\n");
	return;
}

/* INT $0x5 */
__attribute__((interrupt)) static inline void
bounds_check_except(interrupt_frame_t *frame __attribute__((unused)))
{
	k_printf("Exception Raised: Bounds Check Exception.\n");
	return;
}

/* INT $0x6 */
__attribute__((interrupt)) static inline void
invalid_opcode_except(interrupt_frame_t *frame __attribute__((unused)))
{
	k_printf("Exception Raised: Invalid Opcode Exception.\n");
	return;
}

/* INT $0x7 */
__attribute__((interrupt)) static inline void
no_coprocessor_except(interrupt_frame_t *frame __attribute__((unused)))
{
	k_printf("Exception Raised: No Coprocessor Exception.\n");
	return;
}

/* INT $0x8 */
__attribute__((interrupt)) static inline void
double_fault_except(interrupt_frame_t *frame __attribute__((unused)), uint32_t errcode)
{
	k_printf("Exception Raised: Double Fault. ERRCODE: 0x%x\n", errcode);
	__asm__ ( "cli\r\nhlt" );
	return;
}

/* INT $0x9 */
__attribute__((interrupt)) static inline void
coproc_seg_overrun_except(interrupt_frame_t *frame __attribute__((unused)))
{
	k_printf("Exception Raised: Coprocessor Segment Overrun.\n");
	return;
}

/* INT $0xA */
__attribute__((interrupt)) static inline void
invalid_tss_except(interrupt_frame_t *frame __attribute__((unused)), uint32_t errcode)
{
	k_printf("Exception Raised: Invalid TSS. ERRCODE: 0x%x\n", errcode);
	__asm__ ( "cli\r\nhlt" );
	return;
}

/* INT $0xB */
__attribute__((interrupt)) static inline void
no_segment_except(interrupt_frame_t *frame __attribute__((unused)), uint32_t errcode)
{
	k_printf("Exception Raised: Segment Not Present. ERRCODE: 0x%x\n", errcode);
	__asm__ ( "cli\r\nhlt" );
	return;
}

/* INT $0xC */
__attribute__((interrupt)) static inline void
stack_segment_overrun_except(interrupt_frame_t *frame __attribute__((unused)), uint32_t errcode)
{
	k_printf("Exception Raised: Stack Segment Overrun. ERRCODE: 0x%x\n", errcode);
	__asm__ ( "cli\r\nhlt" );
	return;
}

/* INT $0xD */
__attribute__((interrupt)) static inline void
gpf_except(interrupt_frame_t *frame __attribute__((unused)), uint32_t errcode)
{
	k_printf("Exception Raised: General Protection Fault Exception. ERRCODE: 0x%x\n", errcode);
	__asm__ ( "cli\r\nhlt" );
	return;
}

/* INT $0x0E */
__attribute__((interrupt)) static inline void
page_fault_except(interrupt_frame_t *frame __attribute__((unused)), uint32_t errcode)
{
	k_printf("Exception Raised: Page Fault. ERRCODE: 0x%x\n", errcode);
	__asm__ ( "cli\r\nhlt" );
	return;
}

/* INT $0xF */
__attribute__((interrupt)) static inline void
unassigned_except(interrupt_frame_t *frame __attribute__((unused)))
{
	k_printf("Exception Raised: Unassigned. Not Implemented...\n");
	__asm__ ( "cli\r\nhlt" );
	return;
}

/* INT $0x10 */
__attribute__((interrupt)) static inline void
coproc_except(interrupt_frame_t *frame __attribute__((unused)))
{
	k_printf("Exception Raised: Coprocessor Error.\n");
	return;
}

/* INT $0x11 */
__attribute__((interrupt)) static inline void
alignment_check_except(interrupt_frame_t *frame __attribute__((unused)), uint32_t errcode)
{
	k_printf("Exception Raised: Alignment Check Exception. ERRCODE: 0x%x\n", errcode);
	return;
}

/* INT $0x12 */
__attribute__((interrupt)) static inline void
machine_check_except(interrupt_frame_t *frame __attribute__((unused)))
{
	k_printf("Exception Raised: Machine Check Exception.\n");
	return;
}

#endif
