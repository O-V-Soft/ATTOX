#include <video.h>

void panic(const char *panic_message) {
    printk("\nkernel panic: ");
    printk(panic_message);
    printk("\n");
}
