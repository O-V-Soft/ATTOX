#include <stdint.h>
#include <video.h>
#include <idt.h>
#include <regs.h>
#include <fs.h>
#include <task.h>
#include <string.h>
#include <vfs.h>
#include <mm/kheap.h>

void syscall_handler(registers_t *regs) {
    asm volatile("sti");
    
    uint32_t syscall_num = regs->eax;

    switch (syscall_num) {
        case 2: {
            void (*entry_point)() = (void (*)())regs->ebx;

            create_task(entry_point);

            regs->eax = next_pid - 1;
            break;
        }

        case 3:
            regs->eax = vfs_read(regs->ebx, (void*)regs->ecx, regs->edx);
            break;

        case 4:
            regs->eax = vfs_write(regs->ebx, (void*)regs->ecx, regs->edx);
            break;

        case 5: { 
            const char *path = (const char*)regs->ebx;
            int inode = find_inode(path, current_task->cwd);

            if (inode >= 0) {
                int free_fd = -1;
                for (int i = 3; i < 32; i++) {
                    if (fd_table[i] == 0) {
                        free_fd = i;
                        break;
                    }
                }

                if (free_fd >= 0) {
                    if (inodes[inode].type == DEV) {
                        fd_table[free_fd] = &dev_tty_node;
                        regs->eax = free_fd;
                    } else if (inodes[inode].type == FILE) {
                        file_nodes[free_fd].type = inode;
                        file_nodes[free_fd].read = file_read;
                        file_nodes[free_fd].write = file_write;

                        fd_table[free_fd] = &file_nodes[free_fd];
                        regs->eax = free_fd;
                    }
                } else {
                    regs->eax = -1; 
                }
            } else {
                regs->eax = -1; 
            }
            break; 
        }
        
        case 8: { 
            const char *pathname = (const char *)regs->ebx;
            int res = fs_create(pathname, current_task->cwd, FILE);
            regs->eax = (res >= 0) ? 0 : -1;
            break;
        }

        case 12: { 
            const char *path = (const char *)regs->ebx;

            if (strcmp(path, "/") == 0) {
                current_task->cwd = 0;
                regs->eax = 0;
                break;
            }

            int inode = find_inode(path, current_task->cwd);

            if (inode >= 0 && inodes[inode].type == DIR) {
                current_task->cwd = inode; 
                regs->eax = 0;
            } else {
                regs->eax = -1; 
            }
            
            break;
        }

        case 13: { 
            char *buf = (char *)regs->ebx;
            int max_len = regs->ecx;
            int offset = 0;

            for (int i = 1; i < 32; i++) {
                if (dirents[i].used == 1 && dirents[i].parent_id == current_task->cwd) {
                    char *name = dirents[i].name;
                    while (*name && offset < max_len - 3) {
                        buf[offset++] = *name++;
                    }
            
                    buf[offset++] = ' ';
                    buf[offset++] = ' ';
                }
            }
            
            if (offset > 0) {
                buf[offset - 1] = '\n';
            }
            
            buf[offset] = '\0';

            regs->eax = offset;
            break;
        }

        case 39: { 
            const char *pathname = (const char *)regs->ebx;
            int res = fs_create(pathname, current_task->cwd, DIR);
            regs->eax = (res >= 0) ? 0 : -1;
            break;
        }

        default:
            regs->eax = -1;
            break;
    }
}
