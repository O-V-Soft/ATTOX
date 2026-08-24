#include <stdint.h>
#include <idt.h>
#include <task.h>
#include <mm/kheap.h>

task_t *current_task = 0;
static int next_pid = 1;

static task_t main_task;

void task_init() {
    main_task.pid = 0;
    main_task.esp = 0;

    main_task.kernel_esp0 = 0x90000;

    main_task.cwd = 0;
    main_task.next = &main_task;

    current_task = &main_task;
}

void create_task(void (*entry_point)()) {
    task_t *new_task = (task_t*)kmalloc(sizeof(task_t));

    uint8_t *user_stack = (uint8_t*)kmalloc(4096);
    uint32_t user_esp = (uint32_t)user_stack + 4096; 

    uint8_t *kernel_stack = (uint8_t*)kmalloc(4096);
    uint32_t *sp = (uint32_t*)(kernel_stack + 4096);

    *(--sp) = 0x23;                
    *(--sp) = user_esp;           
    *(--sp) = 0x202;              
    *(--sp) = 0x1B;             
    *(--sp) = (uint32_t)entry_point;  

    *(--sp) = 0x23;              
    *(--sp) = 0x23;                
    *(--sp) = 0x23;              
    *(--sp) = 0x23;                  

    for (int i = 0; i < 8; i++) {
        *(--sp) = 0;
    }

    new_task->esp = (uint32_t)sp;
    new_task->kernel_esp0 = (uint32_t)(kernel_stack + 4096);
    new_task->pid = next_pid++;
    new_task->cwd = 0;
    
    new_task->next = current_task->next;
    current_task->next = new_task;
}