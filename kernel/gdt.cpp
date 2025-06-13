#include <cstdint>

struct GDTEntry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

struct GDTDescriptor {
    uint16_t size;
    uint64_t offset;
} __attribute__((packed));

GDTEntry gdt[3];
GDTDescriptor gdt_descriptor;

void init_gdt() {
    gdt[0] = {0, 0, 0, 0, 0, 0}; // Null descriptor
    gdt[1] = {0xffff, 0, 0, 0x9a, 0xcf, 0}; // Code segment
    gdt[2] = {0xffff, 0, 0, 0x92, 0xcf, 0}; // Data segment

    gdt_descriptor.size = sizeof(gdt) - 1;
    gdt_descriptor.offset = (uint64_t)&gdt;

    asm volatile("lgdt %0" : : "m"(gdt_descriptor));