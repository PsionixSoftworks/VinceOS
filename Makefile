.PHONY: all
all : bootloader kernel linker runner cleanup
bootloader:
	i686-elf-as assemb/boot.S -o boot.o
kernel:
	i686-elf-gcc -c init/main.c -o main.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Iinclude
linker:
	i686-elf-gcc -T linker.ld -o VinceOS.bin -ffreestanding -O2 -nostdlib boot.o main.o -lgcc
	mkdir -p isodir/boot/grub
	cp VinceOS.bin isodir/boot/VinceOS.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o VinceOS.iso isodir
runner:
	qemu-system-i386 -cdrom VinceOS.iso
cleanup:
	rm -rf *.o *.bin *.iso
