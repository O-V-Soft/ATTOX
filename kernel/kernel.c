#include <video.h>
#include <stdint.h>
#include <idt.h>
#include <mm/pmm.h>

void __attribute__((section(".text.entry"))) kernel_main() {
    idt_init(); 
    screen_clear();

    pmm_init();

    while(1);
}
