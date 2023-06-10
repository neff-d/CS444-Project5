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

    struct inode *in = ialloc();
 
    int free_block_num = alloc();

    in -> flags = DIR_FLAG_NUM;
    in -> size = ENTRY_SIZE * DIR_BASE_ENTRIES;
    in -> block_ptr[0] = free_block_num;

    unsigned char block[BLOCK_SIZE];

    write_u16(block, in -> inode_num);
    write_u16(block + ENTRY_SIZE, in -> inode_num);

    strcpy((char *) block + FILE_NAME_OFFSET, ".");
    strcpy((char *) block + ENTRY_SIZE + FILE_NAME_OFFSET, "..");

    bwrite(free_block_num, block);
    iput(in);
}