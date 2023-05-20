#include "inode.h"
#include "block.h"
#include "free.h"

int ialloc(void) {

    unsigned char block[BLOCK_SIZE];
    unsigned char *inode_block = bread(INODE_MAP_NUM, block);

    int free_block_num = find_free(inode_block);

    if(free_block_num == -1)
        return -1;
    
    set_free(inode_block, free_block_num, SET);
    bwrite(free_block_num, inode_block);
    
    return free_block_num;
}

int alloc(void) {

    unsigned char block[BLOCK_SIZE];
    unsigned char *data_block = bread(BLOCK_MAP_NUM, block);

    int free_block_num = find_free(data_block);

    if(free_block_num == -1)
        return -1;

    set_free(data_block, free_block_num, SET);
    bwrite(free_block_num, data_block);

    return free_block_num;
}