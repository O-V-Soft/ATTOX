#include <stdint.h>
#include <video.h>
#include <idt.h>
#include <regs.h>
#include <fs.h>

void syscall_handler(registers_t *regs) {
    asm volatile("sti");
    
    uint32_t syscall_num = regs->eax;

    switch (syscall_num) {
        case 3: {
            char *buf = (char*)regs->ecx;

            if (regs->ebx == 0) {
                size_t read_bytes = 0;

                while (read_bytes < regs->edx) {
                    char c = getchar();

                    if (c == '\b') { 
                        if (read_bytes > 0) {
                            read_bytes--; 
                    
                            if (cursor > prompt_limit && cursor >= 2) {
                                cursor -= 2;
                                put_char(' ');
                                cursor -= 2;
                                update_cursor();
                            }
                        }
                        continue; 
                    }

                    buf[read_bytes++] = c;
                    put_char(c);

                    if (c == '\n') break;
                }

                regs->eax = read_bytes;
            } else {
                for (int i = 1; i < 32; i++) {
                    if (dirents[i].used == 1 && dirents[i].parent_id == 0) {
                        printk(dirents[i].name);
                        printk("  ");
                    }
                }
                printk("\n");
                regs->eax = 0;
            }
            break;
        }
        
        case 4: 
            if (regs->ebx == 1 || regs->ebx == 2) { 
                char *str = (char *)regs->ecx;
                printk(str); 
                regs->eax = regs->edx; 
            } else {
                regs->eax = -1; 
            }
            break;

        case 8: { 
            const char *pathname = (const char *)regs->ebx;
            int res = fs_create(pathname, 0, FILE);
            regs->eax = (res >= 0) ? 0 : -1;
            break;
        }

        case 39: { 
            const char *pathname = (const char *)regs->ebx;
            int res = fs_create(pathname, 0, DIR);
            regs->eax = (res >= 0) ? 0 : -1;
            break;
        }

        default:
            regs->eax = -1;
            break;
    }
}
