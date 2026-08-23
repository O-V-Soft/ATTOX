#include <video.h>
#include <stdint.h>
#include <idt.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <mm/kheap.h>
#include <fs.h>
#include <task.h>
#include <unistd.h>
#include <string.h>

void sh() {
    char buf[128];

    while (1) {
        _syscall3(4, 1, (uintptr_t)"sh# ", 4); 

        int bytes = _syscall3(3, 0, (uintptr_t)buf, 127);
        if (bytes <= 0) continue;

        if (buf[bytes - 1] == '\n') {
            buf[bytes - 1] = '\0';
        } else {
            buf[bytes] = '\0';
        }

        if (strcmp(buf, "clear") == 0) {
            screen_clear();
        } 

        else if (strcmp(buf, "help") == 0) {
            const char *help_msg = "help - show this\nclear - clear the screen\n";
            _syscall3(4, 1, (uintptr_t)help_msg, strlen(help_msg));
        } 
        
        else if (buf[0] != '\0') {
            const char *err = "Unknown command\n";
            _syscall3(4, 1, (uintptr_t)err, strlen(err));
        }
    }
}

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

    create_task(sh);
    
    while(1) {
        asm volatile("hlt");
    };
}
