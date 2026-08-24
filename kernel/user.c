#include <video.h>
#include <stdint.h>
#include <idt.h>
#include <mm/kheap.h>
#include <task.h>
#include <usr/tss.h>
#include <usr/user.h>

void switch_to_user_mode(void (*user_func)()) {
    uint8_t *user_stack = (uint8_t*) kmalloc(4096);
    uint32_t user_esp = (uint32_t)user_stack + 4096; 

    __asm__ volatile (
        "cli\n\t"
        "mov $0x23, %%ax\n\t"   
        "mov %%ax, %%ds\n\t"
        "mov %%ax, %%es\n\t"
        "mov %%ax, %%fs\n\t"
        "mov %%ax, %%gs\n\t"

        "pushl $0x23\n\t"       
        "pushl %0\n\t"        
        "pushfl\n\t"           
        "popl %%eax\n\t"
        "orl $0x200, %%eax\n\t"  
        "pushl %%eax\n\t"
        "pushl $0x1B\n\t"     
        "pushl %1\n\t"          
        "iret\n\t"
        :
        : "r"(user_esp), "r"(user_func)
        : "ax", "memory"
    );
}
