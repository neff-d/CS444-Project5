#include <unistd.h>
#include <fcntl.h>

#include "block.h"
#include "image.h"


unsigned char *bread(int block_num, unsigned char *block) {

    off_t offset = block_num * BLOCK_SIZE;

    lseek(image_fd, offset, SEEK_SET);

    read(image_fd, block, BLOCK_SIZE);

    return block;
}

void bwrite(int block_num, unsigned char *block) {

    off_t offset = block_num * BLOCK_SIZE;  

    lseek(image_fd, offset, SEEK_SET);

    write(image_fd, block, BLOCK_SIZE);   
}