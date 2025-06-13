ENTRY(kernel_main)
SECTIONS {
    . = 0x100000; /* Kernel loaded at 1MB */
    .multiboot : {
        *(.multiboot)
    }
    .text : {
        *(.text)
    }
    .data : {
        *(.data)
    }
    .bss : {
        *(.bss)
    }
}