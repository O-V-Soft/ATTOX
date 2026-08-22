#include <stdint.h>
#include <mm/kheap.h>
#include <fs.h>
#include <string.h>

inode_t inodes[32];
dirent_t dirents[32];

void fs_init() {
    for (int i = 0; i < 32; i++) {
        inodes[i].type = 0;
    }

    for (int j = 0; j < 32; j++) {
        dirents[j].used = 0;
    }

    inodes[0].type = DIR;

    dirents[0].name[0] = '/';
    dirents[0].name[1] = '\0';
    dirents[0].used = 1;
    dirents[0].inode_id = 0;
    dirents[0].parent_id = 0;
}

int fs_create(const char *name, uint8_t parent_id, uint8_t type) {
    int free_inode = -1;
    int free_dirent = -1;

    for (int i = 0; i < 32; i++) {
        if (inodes[i].type == FREE) {
            free_inode = i;
            break;
        }
    }

    for (int j = 0; j < 32; j++) {
        if (dirents[j].used == 0) {
            free_dirent = j;
            break;
        }
    }

    if (free_inode == -1 || free_dirent == -1) return -1;

    inodes[free_inode].type = type;
    inodes[free_inode].size = 0;
    inodes[free_inode].data = 0;

    dirents[free_dirent].parent_id = parent_id;
    dirents[free_dirent].inode_id = free_inode;
    dirents[free_dirent].used = 1;
    
    int k = 0;
    while (name[k] != '\0' && k < 15) {
        dirents[free_dirent].name[k] = name[k];
        k++;
    }
    dirents[free_dirent].name[k] = '\0';

    return free_inode;
}

int find_inode(const char *name, uint8_t parent_id) {
    for (int i = 0; i < 32; i++) {
        if (dirents[i].used == 1 && dirents[i].parent_id == parent_id) {
            if (strcmp(name, dirents[i].name) == 0) {
                return dirents[i].inode_id;
            }
        }
    }
    return -1;
}

void fs_write(uint8_t inode_id, const char *buffer, size_t size) {
    if (inode_id >= 32 || inodes[inode_id].type != FILE) return; 

    uint8_t* ptr = (uint8_t*) kmalloc(size);
    if (!ptr) return;

    inodes[inode_id].data = ptr;
    inodes[inode_id].size = size;

    for (size_t i = 0; i < size; i++) {
        inodes[inode_id].data[i] = (uint8_t)buffer[i];
    }
}

int fs_read(uint8_t inode_id, char *buffer, size_t size) {
    if (inode_id >= 32 || inodes[inode_id].type != FILE) return -1;
    if (inodes[inode_id].data == 0) return -1;
    if (size > inodes[inode_id].size) size = inodes[inode_id].size;

    for (size_t i = 0; i < size; i++) {
        buffer[i] = (char)inodes[inode_id].data[i];
    }

    return size;
}
