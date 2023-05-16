#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "block.h"
#include "image.h"


unsigned char *bread(int block_num, unsigned char *block) {

    off_t offset = block_num * BLOCK_SIZE;

    lseek(image_fd, offset, SEEK_SET);

    read(image_fd, block, BLOCK_SIZE);

// lseek() followed by a read()
// multiply the block number by 4096 to get to the right block,
// then read 4096 bytes into the buffer
// SEEK_SET
}

void bwrite(int block_num, unsigned char *block) {

// lseek() to the block, load the 4096 bytes into the buffer, then call write() into image_fd

    
}