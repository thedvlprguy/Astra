// kernel/kernel_main.cpp

// Manually define fixed-width types (since <cstdint> is unavailable)
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

// Multiboot header constants
#define MULTIBOOT_HEADER_MAGIC    0x1BADB002
#define MULTIBOOT_HEADER_FLAGS    0x0
#define MULTIBOOT_HEADER_CHECKSUM (uint32_t)(0 - (MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS))

// Multiboot header structure placed in .multiboot section
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

// Kernel entry point (must use C linkage to avoid name mangling)
extern "C" void kernel_main() {
    uint16_t* vga = (uint16_t*)0xb8000;
    const char* msg = "Welcome to Astra!";
    
    for (int i = 0; msg[i]; i++) {
        vga[i] = (0x0F << 8) | msg[i]; // White text on black background
    }

    while (1) {
        asm volatile("hlt");
    }
}
