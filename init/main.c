#include <video.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

void main() {
    int pid = _syscall0(2);

    sh();
}