#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "inode.h"
#include "block.h"

int ialloc(void) {

    unsigned char block[BLOCK_SIZE];
    unsigned char inode_block = bread(INODE_MAP_NUM, block);

}

int alloc(void) {



    
}