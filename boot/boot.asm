[bits 16]
[org 0x7c00]

start:
    xor ax, ax
    mov ds, ax
    mov ss, ax
    mov sp, 0x7c00
    mov [boot_drive], dl

    in al, 0x92
    or al, 2
    out 0x92, al
    
    mov ax, 0x0000
    mov es, ax
    mov bx, 0x1000
    mov ah, 0x02
    mov al, 50
    mov ch, 0
    mov dh, 0
    mov cl, 2
    mov dl, [boot_drive]
    int 0x13

    cli
    lgdt [gdt_ptr]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp 0x08:init_32bit

[bits 32]
init_32bit:
    mov ax, 0x10
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov fs, ax

    mov esp, 0x90000

    jmp 0x1000

boot_drive: db 0

align 4
gdt_start:
    dq 0             

gdt_code:
    dw 0xFFFF          
    dw 0x0000          
    db 0x00            
    db 10011010b      
    db 11001111b 
    db 0x00      

gdt_data:
    dw 0xFFFF         
    dw 0x0000          
    db 0x00             
    db 10010010b       
    db 11001111b      
    db 0x00     

gdt_end:

gdt_ptr:
    dw gdt_end - gdt_start - 1
    dd gdt_start

times 510-($-$$) db 0
dw 0xAA55