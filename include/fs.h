#ifndef FS_H
#define FS_H
#include <stdint.h>

typedef struct {
    uint8_t type;
    size_t size;
    uint8_t* data;
} inode_t;

typedef struct {
    char name[16];
    uint8_t inode_id;
    uint8_t parent_id;
    uint8_t used;
} dirent_t;

#define FREE 0
#define FILE 1
#define DIR 2

void fs_init();
int fs_create(const char *name, uint8_t parent_id, uint8_t type);
int find_inode(const char *name, uint8_t parent_id);
void fs_write(uint8_t inode_id, const char *buffer, size_t size);
int fs_read(uint8_t inode_id, char *buffer, size_t size);

extern dirent_t dirents[32];

#endif
