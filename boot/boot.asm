[bits 16]
[org 0x7c00]

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00

    ; Print "Astra Booting..."
    mov si, boot_msg
print_loop:
    lodsb
    cmp al, 0
    je done_print
    mov ah, 0x0e
    int 0x10
    jmp print_loop
done_print:

    ; Halt (GRUB will take over)
    cli
    hlt

boot_msg db "Astra Booting...", 0

times 510-($-$$) db 0
dw 0xaa55