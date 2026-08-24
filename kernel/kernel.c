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
#include <usr/user.h>

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

    int dev_id = fs_create("dev", 0, DIR);
    int bin_id = fs_create("bin", 0, DIR);
    int etc_id = fs_create("etc", 0, DIR);

    int hn_id = fs_create("hostname", etc_id, FILE);
    int motd_id = fs_create("motd", etc_id, FILE);
    int fstab_id = fs_create("fstab", etc_id, FILE);

    const char *hostname = "ATTOX";
    fs_write(hn_id, hostname, 6);

    const char *message = "the ATTOX.";
    fs_write(motd_id, message, 11);

    const char *name_fs = "ATTOFS";
    fs_write(fstab_id, name_fs, 7);

    //switch_to_user_mode(sh);
    
    create_task(sh);
    
    while(1) {
        asm volatile("hlt");
    };
}
