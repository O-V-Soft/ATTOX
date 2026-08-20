#ifndef PMM_H
#define PMM_H
#include <stdint.h>

void pmm_init();
uint32_t pmm_alloc_page();
void pmm_free_page(uint32_t page_addr);

#endif 