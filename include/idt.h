#ifndef IDT_H
#define IDT_H
#include <stdint.h>

void set_idt_gate(uint8_t number, uint32_t base, uint16_t selector, uint8_t flags);
void pic_remap();
void idt_init();

void timer_handler();
void keyboard_handler();

#endif 
