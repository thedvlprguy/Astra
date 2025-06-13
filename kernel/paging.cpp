#include <cstdint>

#define PAGE_SIZE 4096

struct PageEntry {
    uint64_t present : 1;
    uint64_t writable : 1;
    uint64_t user : 1;
    uint64_t reserved : 9;
    uint64_t address : 40;
    uint64_t available : 12;
} __attribute__((packed));

PageEntry page_directory[1024] __attribute__((aligned(PAGE_SIZE)));
PageEntry page_table[1024] __attribute__((aligned(PAGE_SIZE)));

void init_paging() {
    for (int i = 0; i < 1024; i++) {
        page_directory[i] = {0, 0, 0, 0, 0, 0};
        page_table[i] = {1, 1, 0, 0, (i * PAGE_SIZE) >> 12, 0};
    }
    page_directory[0] = {1, 1, 0, 0, (uint64_t)&page_table >> 12, 0};

    asm volatile(
        "mov %0, %%cr3\n"
        "mov %%cr0, %%eax\n"
        "or $0x80000000, %%eax\n"
        "mov %%eax, %%cr0"
        : : "r"(&page_directory) : "eax"
    );
}