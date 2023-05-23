#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "block.h"
#include "inode.h"
#include "image.h"
#include "free.h"


unsigned char *bread(int block_num, unsigned char *block) {

    off_t offset = block_num * BLOCK_SIZE;

    if(lseek(image_fd, offset, SEEK_SET) == -1)
        return NULL;

    if(read(image_fd, block, BLOCK_SIZE) != BLOCK_SIZE)
        return NULL;

    return block;
}

void bwrite(int block_num, unsigned char *block) {

    off_t offset = block_num * BLOCK_SIZE;  

    lseek(image_fd, offset, SEEK_SET);
    write(image_fd, block, BLOCK_SIZE);   
}

int alloc(void) {

    unsigned char block[BLOCK_SIZE];
    unsigned char *data_block = bread(BLOCK_MAP_NUM, block);

    int free_block_num = find_free(data_block);
   

    if(free_block_num == -1)
        return -1;

    set_free(data_block, free_block_num, SET);
    bwrite(BLOCK_MAP_NUM, data_block);

    return free_block_num;
}