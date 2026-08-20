#include <stdint.h>
#include <mm/pmm.h>

#define PAGE_SIZE 4096
#define RAM_SIZE 0x400000

#define TOTAL_PAGES (RAM_SIZE / PAGE_SIZE)

uint8_t mem_map[1024]; 

void pmm_init() {
    for (int i = 0; i < 256; i++) {
        mem_map[i] = 1;
    }

    for (int j = 256; j < TOTAL_PAGES; j++) {
        mem_map[j] = 0;
    }
}

uint32_t pmm_alloc_page() {
    for (int i = 256; i < TOTAL_PAGES; i++) {
        if (mem_map[i] == 0) {
            mem_map[i] = 1;
            return i * PAGE_SIZE;
        }
    }

    return 0;
}

void pmm_free_page(uint32_t page_addr) {
    int page_index = page_addr / PAGE_SIZE;

    if (page_index < 256 || page_index >= TOTAL_PAGES) {
        return;
    }

    if (mem_map[page_index] > 0) {
        mem_map[page_index]--;
    }
}