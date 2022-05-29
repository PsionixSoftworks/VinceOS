#include <i386/gdt.h>

struct gdt_entry_bits
{
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_middle;
	uint8_t access;
	uint8_t granularity;
	uint8_t base_high;
} __attribute__((packed));

struct gdt_ptr
{
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

typedef struct gdt_entry_bits 	gdt_t;
typedef struct gdt_ptr 			gdt_ptr_t;

static inline void gdt_add_segment(gdt_t *gdt, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);
static inline void gdt_install(uint32_t pgdt);

void
gdt_init(void)
{
	gdt_t 		gdt[5];
	gdt_ptr_t 	pgdt;

	pgdt.limit 	= (sizeof(gdt_t) * MAX_GDT_ENTRIES) - 1;
	pgdt.base 	= (uint32_t)&gdt;

	gdt_add_segment(gdt, 0, 0, 0, 0);
	gdt_add_segment(gdt, 0x00000000, 0xFFFFFFFF, 0x9A, 0xCF);
	gdt_add_segment(gdt, 0x00000000, 0xFFFFFFFF, 0x92, 0xCF);
	gdt_add_segment(gdt, 0x00000000, 0xFFFFFFFF, 0xFA, 0xCF);
	gdt_add_segment(gdt, 0x00000000, 0xFFFFFFFF, 0xF2, 0xCF);

	gdt_install((uint32_t)&pgdt);
}

static inline void
gdt_add_segment(gdt_t *gdt, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{
	static int 	id = 0;

	if (id < MAX_GDT_ENTRIES)
	{
		gdt[id].base_low 	= (base & 0xFFFF);
		gdt[id].base_middle = (base >> 16) & 0xFF;
		gdt[id].base_high	= (base >> 24) & 0xFF;

		gdt[id].limit_low	= (limit & 0xFFFF);
		gdt[id].granularity	= (limit >> 16) & 0x0F;

		gdt[id].granularity |= granularity & 0xF0;
		gdt[id].access		= access;
		id++;
	}
}

static inline void
gdt_install(uint32_t pgdt)
{
	__asm__ volatile ( "lgdt (%0)" : : "a"(pgdt) );
	__asm__ volatile (
		"movw $0x10, %ax	\n \
		 movw %ax, %ds		\n \
		 movw %ax, %es		\n \
		 movw %ax, %fs		\n \
		 movw %ax, %gs		\n \
		 movw %ax, %ss		\n \
		 ljmp $0x08, $flush	\n \
		 flush: "
	);
}
