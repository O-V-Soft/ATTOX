#include <video.h>
#include <stdint.h>
#include <idt.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <mm/kheap.h>
#include <fs.h>
#include <task.h>
#include <unistd.h>
#include <vfs.h>
#include <usr/tss.h>

void __attribute__((section(".text.entry"))) kernel_main() {
    idt_init(); 
    pmm_init();
    vmm_init();
    fs_init();
    kheap_init();
    task_init();
    vfs_init();
    tss_init();

    screen_clear();
    
    create_task(init);
    
    while(1) {
        asm volatile("hlt");
    };
}
