#ifndef VFS_H
#define VFS_H
#include <stdint.h>

typedef struct node {
    char name[16];
    int type; 
    int (*read)(struct node *node, uint8_t *buffer, int count);
    int (*write)(struct node *node, uint8_t *buffer, int count);
} node_t;

void vfs_init();
int vfs_read(int fd, void *buf, int count);
int vfs_write(int fd, void *buf, int count);

extern node_t *fd_table[32];
extern node_t dev_tty_node;

#endif
