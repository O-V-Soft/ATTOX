#include <video.h>
#include <stdint.h>
#include <idt.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <mm/kheap.h>
#include <fs.h>
#include <task.h>
#include <unistd.h>

void __attribute__((section(".text.entry"))) kernel_main() {
    idt_init(); 
    pmm_init();
    vmm_init();
    fs_init();
    kheap_init();
    task_init();

    screen_clear();

    int dev_id = fs_create("dev", 0, DIR);
    int bin_id = fs_create("bin", 0, DIR);
    int etc_id = fs_create("etc", 0, DIR);
    
    while(1) {
        asm volatile("hlt");
    };
}
