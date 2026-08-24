#ifndef TSS_H
#define TSS_H
#include <stdint.h>

typedef struct {
    uint32_t prev_tss;
    uint32_t esp0;     
    uint32_t ss0;      
    uint32_t esp1;
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
    uint32_t es, cs, ss, ds, fs, gs;
    uint32_t ldt;
    uint16_t trap;
    uint16_t iomap_base;
} __attribute__((packed)) tss_entry_t;

extern tss_entry_t tss;

void tss_init();
void set_kernel_stack(uint32_t stack);

#endif
