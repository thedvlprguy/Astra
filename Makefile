CC = x86_64-elf-gcc
CXX = x86_64-elf-g++
LD = x86_64-elf-ld


ASM = nasm

CFLAGS = -ffreestanding -mno-red-zone -Wall
CXXFLAGS = -ffreestanding -mno-red-zone -Wall -fno-exceptions -fno-rtti
LDFLAGS = -nostdlib -T boot/linker.ld

all: Astra.iso

boot.bin: boot/boot.asm
	$(ASM) -f bin boot/boot.asm -o boot.bin

kernel_main.o: kernel/kernel_main.cpp
	$(CXX) $(CXXFLAGS) -c kernel/kernel_main.cpp -o kernel_main.o

kernel.bin: kernel_main.o
	$(LD) $(LDFLAGS) kernel_main.o -o kernel.bin

Astra.iso: boot.bin kernel.bin
	mkdir -p isodir/boot/grub
	cp kernel.bin isodir/boot/
	cp boot.bin isodir/boot/
	echo 'menuentry "Astra" { multiboot /boot/kernel.bin }' > isodir/boot/grub/grub.cfg
	grub-mkrescue -o Astra.iso isodir

clean:
	rm -rf *.o *.bin Astra.iso isodir

run: Astra.iso
	qemu-system-x86_64 -cdrom Astra.iso -m 512
