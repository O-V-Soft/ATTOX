#include <video.h>
#include <stdint.h>
#include <io.h>

int scancode_to_ascii(uint8_t scancode) {
    int letter = 0;

    switch (scancode) {
        case 0x1C: letter = '\n'; break;
        case 0x1E: letter = 'a'; break;
        case 0x30: letter = 'b'; break;
        case 0x2E: letter = 'c'; break;
        case 0x20: letter = 'd'; break;
        case 0x12: letter = 'e'; break;
        case 0x21: letter = 'f'; break;
        case 0x22: letter = 'g'; break;
        case 0x23: letter = 'h'; break;
        case 0x17: letter = 'i'; break;
        case 0x24: letter = 'j'; break;
        case 0x25: letter = 'k'; break;
        case 0x26: letter = 'l'; break;
        case 0x32: letter = 'm'; break;
        case 0x31: letter = 'n'; break;
        case 0x18: letter = 'o'; break;
        case 0x19: letter = 'p'; break;
        case 0x10: letter = 'q'; break;
        case 0x13: letter = 'r'; break;
        case 0x1F: letter = 's'; break;
        case 0x14: letter = 't'; break;
        case 0x16: letter = 'u'; break;
        case 0x2F: letter = 'v'; break;
        case 0x11: letter = 'w'; break;
        case 0x2D: letter = 'x'; break;
        case 0x15: letter = 'y'; break;
        case 0x2C: letter = 'z'; break;
        case 0x39: letter = ' '; break;
        case 0x02: letter = '1'; break;
        case 0x03: letter = '2'; break;
        case 0x04: letter = '3'; break;
        case 0x05: letter = '4'; break;
        case 0x06: letter = '5'; break;
        case 0x07: letter = '6'; break;
        case 0x08: letter = '7'; break;
        case 0x09: letter = '8'; break;
        case 0x0A: letter = '9'; break;
        case 0x0B: letter = '0'; break;
        case 0x0C: letter = '-'; break;
        case 0x0D: letter = '='; break;
        case 0x34: letter = '.'; break;
        case 0x35: letter = '/'; break;
        case 0x1A: letter = '['; break;
        case 0x1B: letter = ']'; break;
        case 0x33: letter = ','; break;
        case 0x28: letter = '\''; break;
        case 0x27: letter = ';'; break;
        case 0x0E:
			cursor -= 2;
			put_char(' ');
			cursor -= 2;

            update_cursor();

			letter = 0; 
			break;

        default: letter = 0; break;
    }

    return letter;
}

void keyboard_handler() {   
    uint8_t scancode = inb(0x60);

    if (scancode & 0x80) {
        outb(0x20, 0x20);
        return;
    }

    int ascii = scancode_to_ascii(scancode);
    if (ascii) {
        put_char(ascii);
    }

    outb(0x20, 0x20);
}