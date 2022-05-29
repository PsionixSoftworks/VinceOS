ASM			= i686-elf-as
NSM			= nasm -felf32
CC 			= i686-elf-gcc -c
CC_FLAGS 	= -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Iinclude
LINKER		= i686-elf-gcc -T
BINARY		= VinceOS.bin
BIN_FLAGS	= -ffreestanding -O2 -nostdlib -lgcc
ALL_FILES	= boot.o main.o kernel.o vga.o itoa.o strlen.o gdt.o idt.o isr.o pic.o
ISO_OUTPUT	= VinceOS.iso
BUILD		= grub-mkrescue -o $(ISO_OUTPUT)
RUNNER		= qemu-system-i386 -cdrom

RM			= rm -rf

.PHONY: all
all : bootloader kern linker runner cleanup
bootloader:
	$(CC) assemb/boot.S -o boot.o $(CC_FLAGS)
	$(NSM) assemb/isr.asm -o isr.o
kern:
	$(CC) init/main.c -o main.o $(CC_FLAGS)
	$(CC) init/kernel.c -o kernel.o $(CC_FLAGS)
	$(CC) driver/vga.c -o vga.o $(CC_FLAGS)
	$(CC) lib/itoa.c -o itoa.o $(CC_FLAGS)
	$(CC) lib/strlen.c -o strlen.o $(CC_FLAGS)
	$(CC) i386/gdt.c -o gdt.o $(CC_FLAGS)
	$(CC) i386/idt.c -o idt.o $(CC_FLAGS)
	$(CC) kernel/pic.c -o pic.o $(CC_FLAGS)
linker:
	$(LINKER) linker.ld -o $(BINARY) $(BIN_FLAGS) $(ALL_FILES) 
	mkdir -p isodir/boot/grub
	cp VinceOS.bin isodir/boot/VinceOS.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	$(BUILD) isodir
runner:
	$(RUNNER) $(ISO_OUTPUT)
cleanup:
	$(RM) *.o *.bin *.iso *isodir
