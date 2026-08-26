#include <video.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

void sh() {
    char buf[128];
    char name_buf[6];

    _syscall1(12, (uintptr_t)"etc");
    int hn_fd = _syscall1(5, (uintptr_t)"hostname");
    _syscall3(3, hn_fd, (uintptr_t)name_buf, 6);
    _syscall1(12, (uintptr_t)"/");

    while (1) {
        _syscall3(4, 1, (uintptr_t)"\033[32m", 5);

        _syscall3(4, 1, (uintptr_t)"[", 1);
        _syscall3(4, 1, (uintptr_t)"sh", 2);
        _syscall3(4, 1, (uintptr_t)"@", 1);
        _syscall3(4, 1, (uintptr_t)name_buf, 5);
        _syscall3(4, 1, (uintptr_t)"]", 1);
        _syscall3(4, 1, (uintptr_t)"# ", 2);

        _syscall3(4, 1, (uintptr_t)"\033[0m", 4);

        prompt_limit = cursor;

        int bytes = _syscall3(3, 0, (uintptr_t)buf, 127);
        if (bytes <= 0) continue;

        if (buf[bytes - 1] == '\n') {
            buf[bytes - 1] = '\0';
        } else {
            buf[bytes] = '\0';
        }

        if (buf[0] == '\0') continue;

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
            char dir_buf[256];
            int bytes = _syscall2(13, (uintptr_t)dir_buf, sizeof(dir_buf));
            
            if (bytes > 0) {
                _syscall3(4, 1, (uintptr_t)dir_buf, bytes);
            }
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

        else if (buf[0] == 'c' && buf[1] == 'a' && buf[2] == 't' && buf[3] == ' ') {
            char *filename = buf + 4;
            while (*filename == ' ') filename++; 

            int fd = _syscall1(5, (uintptr_t)filename);
    
            if (fd < 0) {
                const char *err = "cat: no such file\n";
                _syscall3(4, 1, (uintptr_t)err, strlen(err));
            } else {
                char read_buf[256];
        
                int bytes_read = _syscall3(3, fd, (uintptr_t)read_buf, sizeof(read_buf) - 1);
        
                if (bytes_read > 0) {
                    read_buf[bytes_read] = '\0';
                    _syscall3(4, 1, (uintptr_t)read_buf, bytes_read);
            
                    if (read_buf[bytes_read - 1] != '\n') {
                        _syscall3(4, 1, (uintptr_t)"\n", 1);
                    }
                }
            }
        }
            
        else if (strcmp(buf, "help") == 0) {
            const char *help_msg = "cd\ncat <file>\nls\ntouch <file>\nmkdir <dir>\nhelp\n";
            _syscall3(4, 1, (uintptr_t)help_msg, strlen(help_msg));
        } 
            
        else {
            const char *err = "Unknown command\n";
            _syscall3(4, 1, (uintptr_t)err, strlen(err));
        }
    }
}
