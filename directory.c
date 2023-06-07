#include <stdlib.h>
#include <string.h>
#include "directory.h"
#include "inode.h"
#include "block.h"
#include "pack.h"


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