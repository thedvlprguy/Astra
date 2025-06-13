#include <cstdint>

void init_keyboard() {
    // Enable keyboard interrupt (IRQ1)
    outb(0x21, inb(0x21) & ~0x02);
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

extern "C" void keyboard_isr() {
    uint8_t scancode = inb(0x60);
    uint16_t* vga = (uint16_t*)0xb8000;
    vga[160] = (0x0f << 8) | scancode; // Display scancode
    outb(0x20, 0x20); // EOI
}