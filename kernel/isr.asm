[bits 32]
extern keyboard_isr
extern isr_handler

global isr0
isr0:
    pusha
    call isr_handler
    popa
    iret

global keyboard_isr_entry
keyboard_isr_entry:
    pusha
    call keyboard_isr
    popa
    iret