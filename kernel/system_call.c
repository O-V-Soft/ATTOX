#include <stdint.h>
#include <video.h>
#include <idt.h>
#include <regs.h>

void syscall_handler(registers_t *regs) {
    uint32_t syscall_num = regs->eax;

    switch (syscall_num) {
        case 4: 
            if (regs->ebx == 1) { 
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
