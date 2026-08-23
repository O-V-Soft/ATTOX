#ifndef UNISTD_H
#define UNISTD_H
#include <stdint.h>

static inline int _syscall0(int num) {
    int ret;
    __asm__ __volatile__ (
        "int $0x80"
        : "=a" (ret)
        : "a" (num)
        : "memory"
    );
    return ret;
}

static inline int _syscall1(int num, uintptr_t arg1) {
    int ret;
    __asm__ __volatile__ (
        "int $0x80"
        : "=a" (ret)
        : "a" (num), "b" (arg1)
        : "memory"
    );
    return ret;
}

static inline int _syscall2(int num, uintptr_t arg1, uintptr_t arg2) {
    int ret;
    __asm__ __volatile__ (
        "int $0x80"
        : "=a" (ret)
        : "a" (num), "b" (arg1), "c" (arg2)
        : "memory"
    );
    return ret;
}

static inline int _syscall3(int num, uintptr_t arg1, uintptr_t arg2, uintptr_t arg3) {
    int ret;
    __asm__ __volatile__ (
        "int $0x80"
        : "=a" (ret)
        : "a" (num), "b" (arg1), "c" (arg2), "d" (arg3)
        : "memory"
    );
    return ret;
}

#endif
