#ifndef VMM_H
#define VMM_H
#include <stdint.h>

extern void load_page_directory(uint32_t* page_directory);
extern void enable_paging();

void vmm_init();
void map_page(uint32_t virtual_addr, uint32_t physical_addr);

#endif
