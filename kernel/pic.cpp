void init_pic() {
    // Initialize PIC
    outb(0x20, 0x11); // ICW1: Initialize
    outb(0xa0, 0x11);
    outb(0x21, 0x20); // ICW2: Vector offset
    outb(0xa1, 0x28);
    outb(0x21, 0x04); // ICW3: Master/Slave
    outb(0xa1, 0x02);
    outb(0x21, 0x01); // ICW4: 8086 mode
    outb(0xa1, 0x01);
    outb(0x21, 0x0);  // Enable interrupts
    outb(0xa1, 0x0);
}

void outb(uint16_t port, uint8_t value) {
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}