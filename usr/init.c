#include <video.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

void init() {
    int etc_id = _syscall2(39, (uintptr_t)"etc", 075);

    _syscall1(12, (uintptr_t)"etc");

    int hn_id = _syscall2(8, (uintptr_t)"hostname", 0755);
    int motd_id = _syscall2(8, (uintptr_t)"motd", 0755);
    int fstab_id = _syscall2(8, (uintptr_t)"fstab", 0755);

    int hn_fd = _syscall1(5, (uintptr_t)"hostname");
    _syscall3(4, hn_fd, (uintptr_t)"ATTOX", 6);

    int motd_fd = _syscall1(5, (uintptr_t)"motd");
    _syscall3(4, motd_fd, (uintptr_t)"the ATTOX.", 11);

    int fstab_fd = _syscall1(5, (uintptr_t)"fstab");
    _syscall3(4, fstab_fd, (uintptr_t)"ATTOFS", 7);

    int sh_pid = _syscall1(2, (uint32_t)sh);

    while (1) {
        asm volatile("nop");
    }
}