#include <stdint.h>
#include <idt.h>
#include <task.h>
#include <usr/tss.h>

uint32_t schedule(uint32_t current_esp) {
    if (!current_task) return current_esp;

    current_task->esp = current_esp;

    current_task = current_task->next;

    set_kernel_stack(current_task->kernel_esp0);

    return current_task->esp;
}