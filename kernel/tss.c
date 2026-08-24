#include <stdint.h>
#include <usr/tss.h>

tss_entry_t tss;

typedef struct {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed)) gdt_entry_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdtr_t;

void tss_init() {
    uint8_t *ptr = (uint8_t *)&tss;
    for (size_t i = 0; i < sizeof(tss); i++) {
        ptr[i] = 0;
    }

    tss.ss0 = 0x10;      
    tss.esp0 = 0x90000; 

    gdtr_t gdtr;
    __asm__ volatile("sgdt %0" : "=m"(gdtr));

    gdt_entry_t *gdt = (gdt_entry_t *)gdtr.base;
    gdt_entry_t *tss_desc = &gdt[5];

    uint32_t base = (uint32_t)&tss;
    tss_desc->base_low    = base & 0xFFFF;
    tss_desc->base_middle = (base >> 16) & 0xFF;
    tss_desc->base_high   = (base >> 24) & 0xFF;

    __asm__ volatile("ltr %%ax" : : "a"(0x28));
}

void set_kernel_stack(uint32_t stack) {
    tss.esp0 = stack;
}
