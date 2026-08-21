#include <stdint.h>
#include <mm/pmm.h>
#include <mm/kheap.h>

static uint32_t heap_curr = 0;
static uint32_t heap_end = 0;

void kheap_init() {
    heap_curr = pmm_alloc_page();
    heap_end = heap_curr + 4096;
}

void *kmalloc(size_t size) {
    if (size == 0) return 0;

    while (heap_curr + size > heap_end) {
        uint32_t page = pmm_alloc_page();
        if (!page) return 0; 
        heap_end = page + 4096;
    }

    void *ptr = (void *)heap_curr;
    heap_curr = heap_curr + size;
    return ptr;
}

void kfree(void *ptr) {
  (void)ptr;
}
