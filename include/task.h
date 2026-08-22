#ifndef TASK_H
#define TASK_H
#include <stdint.h>

typedef struct task {
    uint32_t esp;
    int pid;
    struct task *next;
} task_t;

void task_init();
void create_task(void (*entry_point)());
uint32_t schedule(uint32_t current_esp);

extern task_t *current_task;

#endif