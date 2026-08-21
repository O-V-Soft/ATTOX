#include <video.h>

void fault_handler() {
    panic("Double fault");

    while (1) {
        asm volatile("hlt");
    }
}
