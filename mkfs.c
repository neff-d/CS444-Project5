#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "mkfs.h"
#include "image.h"
#include "inode.h"
#include "free.h"
#include "block.h"



void mkfs(void) {

    unsigned char buffer[BLOCK_SIZE] = {0};

    for(int i = 0; i < NUM_BLOCKS; i++)            
        write(image_fd, buffer, sizeof buffer);

    for(int i = 0; i < 7; i++) {
        alloc();
    }
}