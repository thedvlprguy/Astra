CC = x86_64-elf-gcc
CXX = x86_64-elf-g++
LD = x86_64-elf-ld

ASM = nasm

CFLAGS = -ffreestanding -mno-red-zone -Wall
CXXFLAGS = -ffreestanding -mno-red-zone -Wall -fno-exceptions -fno-rtti
LDFLAGS = -nostdlib -T boot/linker.ld

ISO_NAME = Astra.iso

all: $(ISO_NAME)

boot.bin: boot/boot.asm
	$(ASM) -f bin boot/boot.asm -o boot.bin

kernel_main.o: kernel/kernel_main.cpp
	$(CXX) $(CXXFLAGS) -c kernel/kernel_main.cpp -o kernel_main.o

ata.o: drivers/ata.cpp
	$(CXX) $(CXXFLAGS) -c drivers/ata.cpp -o ata.o

ext2.o: fs/ext2.cpp
	$(CXX) $(CXXFLAGS) -c fs/ext2.cpp -o ext2.o

kernel.bin: kernel_main.o ata.o ext2.o
	$(LD) $(LDFLAGS) kernel_main.o ata.o ext2.o -o kernel.bin

$(ISO_NAME): boot.bin kernel.bin
	mkdir -p isodir/boot/grub
	cp kernel.bin isodir/boot/
	cp boot.bin isodir/boot/
	echo 'menuentry "Astra" { multiboot /boot/kernel.bin }' > isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO_NAME) isodir

clean:
	rm -rf *.o *.bin $(ISO_NAME) isodir

run: $(ISO_NAME) disk.img
	qemu-system-x86_64 -cdrom $(ISO_NAME) -hda disk.img -m 512
