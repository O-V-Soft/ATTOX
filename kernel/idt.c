#include <video.h>
#include <stdint.h>
#include <io.h>
#include <idt.h>
#include <task.h>

struct idt_entry {
    uint16_t addr_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t addr_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t idt_size;
    uint32_t idt_ptr;
} __attribute__((packed));

struct idt_entry   idt_entry[256];
struct idt_ptr idt_ptr;

extern void timer_wrapper();
extern void keyboard_wrapper();
extern void fault_wrapper();
extern void syscall_wrapper();

void set_idt_gate(uint8_t number, uint32_t base, uint16_t selector, uint8_t flags) {
    idt_entry[number].addr_low = (base & 0xFFFF);
    idt_entry[number].addr_high = (base >> 16) & 0xFFFF;
    idt_entry[number].selector = selector;
    idt_entry[number].zero = 0;
    idt_entry[number].flags = flags;
}

void pic_remap() {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    outb(0x21, 0xFC);
    outb(0xA1, 0xFF);
}

void idt_init() {
    idt_ptr.idt_size = sizeof(struct idt_entry) * 256 - 1;
    idt_ptr.idt_ptr = (uint32_t)&idt_entry;

    pic_remap();

    for (int i = 0; i < 256; i++) {
        set_idt_gate(i, (uint32_t)0, 0x08, 0x8E);
    }

    set_idt_gate(8, (uint32_t)fault_wrapper, 0x08, 0x8E);
    set_idt_gate(32, (uint32_t)timer_wrapper, 0x08, 0x8E);
    set_idt_gate(33, (uint32_t)keyboard_wrapper, 0x08, 0x8E);
    set_idt_gate(128, (uint32_t)syscall_wrapper, 0x08, 0xEE);

    asm volatile("lidt (%0)" : : "r" (&idt_ptr)); 
    asm volatile("sti");
}
