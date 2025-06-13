#include <cstdint>

struct IDTEntry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t reserved;
} __attribute__((packed));

struct IDTDescriptor {
    uint16_t size;
    uint64_t offset;
} __attribute__((packed));

IDTEntry idt[256];
IDTDescriptor idt_descriptor;

extern "C" void isr_handler() {
    // Placeholder ISR
    uint16_t* vga = (uint16_t*)0xb8000;
    const char* msg = "Interrupt triggered!";
    for (int i = 0; msg[i]; i++) {
        vga[i + 80] = (0x0f << 8) | msg[i];
    }
}

void init_idt() {
    idt[0] = {0, 0x08, 0, 0x8e, 0, 0, 0}; // Dummy ISR
    idt_descriptor.size = sizeof(idt) - 1;
    idt_descriptor.offset = (uint64_t)&idt;

    asm volatile("lidt %0" : : "m"(idt_descriptor));