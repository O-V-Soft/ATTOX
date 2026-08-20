#include <video.h>
#include <stdint.h>

void panic(const char *panic_message) {
    screen_clear();
    printk("kernel panic: ");
    printk(panic_message);
    printk("\n");
}