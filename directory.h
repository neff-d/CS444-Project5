#ifndef DIRECTORY_H
#define DIRECTORY_H

#define ENTRY_SIZE        32
#define DIR_BASE_ENTRIES   2
#define DIR_BASE_OFFSET    0
#define ENTRY_NAME_OFFSET  2


struct directory {
    struct inode *inode;
    unsigned int offset;
};

struct directory_entry {
    unsigned int inode_num;
    char name[16];
};

void mkfs(void);
struct directory *directory_open(int inode_num);
void directory_close(struct directory *d);
int directory_get(struct directory *dir, struct directory_entry *ent);

#endif