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
            task_t *child = (task_t*)kmalloc(sizeof(task_t));
            child->pid = next_pid++;
            child->cwd = current_task->cwd;

            uint8_t *child_kstack = (uint8_t*)kmalloc(4096);
            uint8_t *parent_kstack_base = (uint8_t*)(current_task->kernel_esp0 - 4096);

            for (int i = 0; i < 4096; i++) {
                child_kstack[i] = parent_kstack_base[i];
            }

            uint32_t esp_offset = (uint32_t)parent_kstack_base + 4096 - (uint32_t)regs;
            uint32_t child_kstack_top = (uint32_t)child_kstack + 4096;

            child->kernel_esp0 = child_kstack_top;

            registers_t *child_regs = (registers_t*)(child_kstack_top - esp_offset);

            child_regs->eax = 0;

            child->esp = (uint32_t)child_regs;
            child->next = current_task->next;
            current_task->next = child;

            regs->eax = child->pid;
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
