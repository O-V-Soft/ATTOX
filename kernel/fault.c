void panic(const char *panic_message);

void fault_handler() {
    panic("\nDouble fault");

    while (1) {
        asm volatile("hlt");
    }
}