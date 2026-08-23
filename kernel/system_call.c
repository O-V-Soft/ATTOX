#include <stdint.h>
#include <video.h>
#include <idt.h>
#include <regs.h>

void syscall_handler(registers_t *regs) {
    asm volatile("sti");
    
    uint32_t syscall_num = regs->eax;

    switch (syscall_num) {
        case 3:
            char *buf = (char*)regs->ecx;

            if (regs->ebx == 0) {
                size_t read_bytes = 0;

                while (read_bytes < regs->edx) {
                    char c = getchar();
                    buf[read_bytes++] = c;

                    put_char(c);

                    if (c == '\n') break;
                }

                regs->eax = read_bytes;
            } else {
                regs->eax = -1;
            }
            break;
        case 4: 
            if (regs->ebx == 1 || regs->ebx == 2) { 
                char *str = (char *)regs->ecx;
                printk(str); 
                regs->eax = regs->edx; 
            } else {
                regs->eax = -1; 
            }
            break;

        default:
            regs->eax = -1;
            break;
    }
}
