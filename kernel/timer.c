#include <video.h>
#include <stdint.h>
#include <io.h>

static uint32_t tick = 0;

void timer_handler() {
    tick++;
    outb(0x20, 0x20);
}