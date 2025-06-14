#include "../fs/ext2.hpp"

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

#define MULTIBOOT_HEADER_MAGIC 0x1BADB002
#define MULTIBOOT_HEADER_FLAGS 0x0
#define MULTIBOOT_HEADER_CHECKSUM (uint32_t)(0 - (MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS))

__attribute__((section(".multiboot")))
struct {
    uint32_t magic;
    uint32_t flags;
    uint32_t checksum;
} multiboot_header = {
    MULTIBOOT_HEADER_MAGIC,
    MULTIBOOT_HEADER_FLAGS,
    MULTIBOOT_HEADER_CHECKSUM
};

extern "C" void kernel_main() {
    uint16_t* vga = (uint16_t*)0xb8000;
    const char* msg = "Welcome to Astra!";
    for (int i = 0; msg[i]; i++) {
        vga[i] = (0x0f << 8) | msg[i];
    }

    // Mount filesystem
    if (fs.mount()) {
        const char* fs_msg = "EXT2 mounted!";
        for (int i = 0; fs_msg[i]; i++) {
            vga[80 + i] = (0x0f << 8) | fs_msg[i]; // Second line
        }
        fs.list_root_dir(); // List root directory
    } else {
        const char* err_msg = "Failed to mount EXT2!";
        for (int i = 0; err_msg[i]; i++) {
            vga[80 + i] = (0x0f << 8) | err_msg[i];
        }
    }

    while (1) {
        asm volatile("hlt");
    }
}
