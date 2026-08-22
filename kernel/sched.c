#include <stdint.h>
#include <idt.h>
#include <task.h>

uint32_t schedule(uint32_t current_esp) {
    if (!current_task) return current_esp;

    current_task->esp = current_esp;

    current_task = current_task->next;

    return current_task->esp;
}