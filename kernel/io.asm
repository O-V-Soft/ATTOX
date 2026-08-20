[bits 32]

global outb
global inb

outb:
    mov edx, [esp + 4]
    mov al, [esp + 8]
    out dx, al

inb:
    mov edx, [esp + 4]
    in al, dx
    ret
    
