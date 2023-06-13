#include <stdlib.h>
#include <string.h>

#include "directory.h"
#include "inode.h"
#include "block.h"
#include "pack.h"
#include "dirbasename.h"
#include "mkfs.h"


struct directory *directory_open(int inode_num) {

    struct inode *in = iget(inode_num);

    if(in == NULL)
        return NULL;

    struct directory *dir = malloc(sizeof(struct directory));

    dir -> inode = in;
    dir -> offset = DIR_BASE_OFFSET;

    return dir;
}

int directory_get(struct directory *dir, struct directory_entry *ent) {

    unsigned int offset = dir -> offset;

    if(offset >= dir -> inode -> size)
        return -1;

    unsigned int data_block_index = offset / BLOCK_SIZE;
    unsigned short data_block_num = dir -> inode -> block_ptr[data_block_index];

    unsigned char block[BLOCK_SIZE];
    bread(data_block_num, block);

    int offset_in_block = offset % BLOCK_SIZE;

    unsigned short ent_inode_num = read_u16(block + offset_in_block);
    ent -> inode_num = ent_inode_num;
    strcpy(ent -> name, (char *) block + offset_in_block + FILE_NAME_OFFSET);

    dir -> offset += ENTRY_SIZE;

    return 0;
}

void directory_close(struct directory *d) {

    iput(d -> inode);
    free(d);
}

struct inode *namei(char *path) {

    struct inode *in = iget(ROOT_INODE_NUM);

    char base_path[BLOCK_SIZE];

    get_basename(path, base_path);

    if(strcmp("/", base_path) == 0)
        return in;

    return NULL;
}

int directory_make(char *path) {

    char base_path[BLOCK_SIZE];

    get_basename(path, base_path);

    char dir_name[BLOCK_SIZE];
    
    get_dirname(path, dir_name);

    struct inode *parent = namei(dir_name);
    struct inode *in = ialloc();

    if(in == NULL)
        return -1;

    int data_block_num = alloc();

    if(data_block_num == -1) {
        iput(in);
        return -1;
    }

    char block[BLOCK_SIZE];

    write_u16(block, in -> inode_num);
    write_u16(block + ENTRY_SIZE, parent -> inode_num);

    strcpy((char *) block + FILE_NAME_OFFSET, ".");
    strcpy((char *) block + ENTRY_SIZE + FILE_NAME_OFFSET, "..");

    in -> size = ENTRY_SIZE * DIR_BASE_ENTRIES;
    in -> flags = DIR_FLAG_NUM;
    in -> block_ptr[0] = data_block_num;

    bwrite(data_block_num, block);

    int block_index = parent -> size / BLOCK_SIZE;
    int block_num = parent -> block_ptr[block_index];

    bread(block_num, block);

    write_u16(block + parent -> size, in -> inode_num);
    strcpy(block + parent -> size + FILE_NAME_OFFSET, base_path);

    parent -> size += ENTRY_SIZE;

    bwrite(block_num, block);

    iput(in);
    iput(parent);

    return 0;
}