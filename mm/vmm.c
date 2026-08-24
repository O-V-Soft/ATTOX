#include <stdint.h>
#include <io.h>
#include <video.h>
#include <mm/pmm.h>
#include <mm/vmm.h>

#define PAGE_PRESENT (1 << 0)
#define PAGE_RW      (1 << 1)
#define PAGE_USER    (1 << 2)

#define PAGE_DEFAULT_USER (PAGE_PRESENT | PAGE_RW | PAGE_USER) 

uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));

void vmm_init() {
    for (int i = 0; i < 1024; i++) {
        page_directory[i] = PAGE_PRESENT | PAGE_RW;
    }

    for (int j = 0; j < 1024; j++) {
        uint32_t phys_addr = j * 4096;
        first_page_table[j] = phys_addr | PAGE_DEFAULT_USER;
    }

    page_directory[0] = ((uint32_t)first_page_table) | PAGE_DEFAULT_USER;

    load_page_directory(page_directory);
    enable_paging();
}

void map_page(uint32_t virtual_addr, uint32_t physical_addr) {
    uint16_t page_directory_index = (virtual_addr >> 22) & 0x3FF;
    uint16_t page_table_index = (virtual_addr >> 12) & 0x3FF;

    if (!(page_directory[page_directory_index] & 1)) {
        uint32_t phys_paddr = pmm_alloc_page();
        
        uint32_t* new_page_table = (uint32_t*)phys_paddr;
        
        for (int i = 0; i < 1024; i++) {
            new_page_table[i] = PAGE_PRESENT | PAGE_RW;
        }
        
        page_directory[page_directory_index] = phys_paddr | PAGE_DEFAULT_USER;
    }

    uint32_t* page_table = (uint32_t*)(page_directory[page_directory_index] & ~0xFFF);
    page_table[page_table_index] = (physical_addr & ~0xFFF) | PAGE_DEFAULT_USER;
}