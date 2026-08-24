#include <video.h>
#include <stdint.h>
#include <fs.h>
#include <unistd.h>
#include <string.h>

void sh() {
    char buf[128];

    char *dir = "dev"; 
    int to_dev = _syscall1(12, (uintptr_t)dir);

    int tty_fd = _syscall2(5, (uintptr_t)"tty", 0);

    dir = "/";
    int to_root = _syscall1(12, (uintptr_t)dir);

    while (1) {
        _syscall3(4, 1, (uintptr_t)"sh# ", 4); 

        prompt_limit = cursor;

        int bytes = _syscall3(3, 0, (uintptr_t)buf, 127);
        if (bytes <= 0) continue;

        if (buf[bytes - 1] == '\n') {
            buf[bytes - 1] = '\0';
        } else {
            buf[bytes] = '\0';
        }

        if (buf[0] == '\0') continue;

        if (strcmp(buf, "clear") == 0) {
            screen_clear();
        } 

        else if (buf[0] == 'c' && buf[1] == 'd' && (buf[2] == ' ' || buf[2] == '\0')) {
            char *path = buf + 3;
            if (buf[2] == '\0' || path[0] == '\0') {
                path = "/"; 
            }

            int res = _syscall1(12, (uintptr_t)path);
            if (res < 0) {
                const char *err = "cd: no such directory\n";
                _syscall3(4, 1, (uintptr_t)err, strlen(err));
            }
        }

        else if (strcmp(buf, "ls") == 0) {
             _syscall3(3, 1, 0, 0);
        } 

        else if (buf[0] == 't' && buf[1] == 'o' && buf[2] == 'u' && buf[3] == 'c' && buf[4] == 'h' && buf[5] == ' ') {
            char *filename = buf + 6;
            int res = _syscall2(8, (uintptr_t)filename, 0755); 
            if (res < 0) {
                const char *err = "touch: failed to create file\n";
                _syscall3(4, 1, (uintptr_t)err, strlen(err));
            }
        } 

        else if (buf[0] == 'm' && buf[1] == 'k' && buf[2] == 'd' && buf[3] == 'i' && buf[4] == 'r' && buf[5] == ' ') {
            char *dirname = buf + 6;
            int res = _syscall2(39, (uintptr_t)dirname, 0755); 
            if (res < 0) {
                const char *err = "mkdir: failed to create directory\n";
                _syscall3(4, 1, (uintptr_t)err, strlen(err));
            }
        } 
            
        else if (strcmp(buf, "help") == 0) {
            const char *help_msg = "cd\nls\ntouch <file>\nmkdir <dir>\nclear\nhelp\n";
            _syscall3(4, 1, (uintptr_t)help_msg, strlen(help_msg));
        } 
            
        else {
            const char *err = "Unknown command\n";
            _syscall3(4, 1, (uintptr_t)err, strlen(err));
        }
    }
}
