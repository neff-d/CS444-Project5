#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "block.h"


unsigned char *bread(int block_num, unsigned char *block) {

// lseek()
// multiply the block number by 4096 to get to the right block,
// then read 4096 bytes into the buffer

}

void bwrite(int block_num, unsigned char *block) {

// lseek() to the block, load the 4096 bytes into the buffer, then call write() into image_fd

    
}