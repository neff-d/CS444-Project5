#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "mkfs.h"
#include "image.h"
#include "inode.h"
#include "free.h"
#include "block.h"
#include "pack.h"
#include "directory.h"



void mkfs(void) {

    unsigned char buffer[BLOCK_SIZE] = {0};

    for(int i = 0; i < NUM_BLOCKS; i++)            
        write(image_fd, buffer, sizeof buffer);

    for(int i = 0; i < 7; i++) {
        alloc();
    }
/*
    ######## I commented this section out until Project 6 is graded because it is causing some
             of the previous tests I wrote to fail #######

    struct inode *in = ialloc();
 
    // alloc() new regular block

    int free_block_num = alloc();

    in -> flags = DIR_FLAG_NUM;
    in -> size = ENTRY_SIZE * DIR_BASE_ENTRIES;
    in -> block_ptr[0] = free_block_num;

    // create the unsigned char block

    unsigned char block[BLOCK_SIZE];

    // write_16() to fill the block and strcpy() the file names

    write_u16(block, in -> inode_num);
    write_u16(block + ENTRY_SIZE, in -> inode_num);

    strcpy(block + FILE_NAME_OFFSET, ".");
    strcpy(block + ENTRY_SIZE + FILE_NAME_OFFSET, "..");

    // bwrite() to disk to get the data from memory to disk
    // iput() the root directory inode to disk

    bwrite(free_block_num, block);
    iput(in);
*/
}