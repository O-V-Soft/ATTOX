#include <stdint.h>
#include <vfs.h>
#include <video.h>
#include <idt.h>

int tty_read(node_t *node, uint8_t *buffer, int count) {
    int read_bytes = 0;

    while (read_bytes < count) {
        char c = getchar();

        if (c == '\b') { 
            if (read_bytes > 0) {
                read_bytes--; 
        
                if (cursor > prompt_limit && cursor >= 2) {
                    cursor -= 2;
                    put_char(' ');
                    cursor -= 2;
                    update_cursor();
                }
            }
            continue; 
        }

        buffer[read_bytes++] = c;
        put_char(c);

        if (c == '\n') break;
    }

    return read_bytes;
}

int tty_write(node_t *node, uint8_t *buffer, int count) {
    for (int i = 0; i < count; i++) {
      put_char(buffer[i]);
    }

  return count;
}

node_t dev_tty_node = {
    .name = "tty",
    .type = 3,
    .read = tty_read,
    .write = tty_write
};

node_t *fd_table[32];

void vfs_init() {
    for (int i = 0; i < 32; i++) {
       fd_table[i] = 0;
    }

    fd_table[0] = &dev_tty_node;
    fd_table[1] = &dev_tty_node;
    fd_table[2] = &dev_tty_node;
}

int vfs_read(int fd, void *buf, int count) {
    if (fd < 32 && fd_table[fd] && fd_table[fd]->read) {
        return fd_table[fd]->read(fd_table[fd], (uint8_t*)buf, count);
    }

    return -1;
}

int vfs_write(int fd, void *buf, int count) {
    if (fd < 32 && fd_table[fd] && fd_table[fd]->write) {
        return fd_table[fd]->write(fd_table[fd], (uint8_t*)buf, count);
    }

    return -1;
}
