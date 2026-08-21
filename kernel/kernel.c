#include <video.h>
#include <stdint.h>
#include <idt.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <mm/kheap.h>
#include <fs.h>

void __attribute__((section(".text.entry"))) kernel_main() {
    idt_init(); 
    pmm_init();
    vmm_init();
    fs_init();
    kheap_init();

    screen_clear();

    char buf[1024];

    int home_id = fs_create("home", 0, DIR); 
    int welcome_id = fs_create("welcome", home_id, FILE); 

    fs_write(welcome_id, "welcome to the attox!", 13); 

    int id = find_inode("sh", home_id);

    if (id != -1) {
        int bytes = fs_read(id, buf, 13);
        buf[bytes] = '\0';
        printk(buf);
        printk("\n");
    } else {
        panic("welcome file not found!\n");
    }

    while(1) {
        asm volatile("hlt");
    };
}
