#include <video.h>
#include <stdint.h>
#include <io.h>

uint8_t *vga = (uint8_t*)0xb8000;

int cursor = 0;
uint8_t color = 0x0F;

void screen_clear() {
	cursor = 0;
	for (int i = 0; i < 2000; i++) {
		put_char(' ');
	}
	cursor = 0;
}

void update_cursor() {
    uint16_t pos = cursor / 2;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));

    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void put_char(char c) {
    if (c == '\n') {
		int current_row = cursor / 160;

		cursor = (current_row + 1) * 160;
	}
	else {
		vga[cursor] = c;
		vga[cursor + 1] = color;
		cursor += 2;
	}

	update_cursor();
}

void printk(const char *msg) {
	for (int i = 0; msg[i] != 0; i++) {
		put_char(msg[i]);
	}
}
