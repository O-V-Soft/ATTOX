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
    main_task.next = &main_task;

    current_task = &main_task;
}

void create_task(void (*entry_point)()) {
    task_t *new_task = (task_t*)kmalloc(sizeof(task_t));

    uint8_t *stack = (uint8_t*)kmalloc(4096);
    uint32_t *sp = (uint32_t*)(stack + 4096);

    *(--sp) = 0x0202;
    *(--sp) = 0x08;
    *(--sp) = (uint32_t)entry_point;

    for (int i = 0; i < 8; i++) {
        *(--sp) = 0;
    }

    new_task->esp = (uint32_t)sp;
    new_task->pid = next_pid++;
    
    new_task->next = current_task->next;
    current_task->next = new_task;
}