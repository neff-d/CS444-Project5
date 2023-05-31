#include <stdlib.h>
#include "inode.h"
#include "block.h"
#include "free.h"
#include "pack.h"

static struct inode incore[MAX_SYS_OPEN_FILES] = {0};

int ialloc(void) {

    unsigned char block[BLOCK_SIZE];
    unsigned char *inode_block = bread(INODE_MAP_NUM, block);

    int free_block_num = find_free(inode_block);

    if(free_block_num == -1)
        return -1;
    
    set_free(inode_block, free_block_num, SET);
    bwrite(INODE_MAP_NUM, inode_block);
    
    return free_block_num;
}

struct inode *find_incore_free(void) {

    for(int i = 0; i < MAX_SYS_OPEN_FILES; i++) {

        if(incore[i].ref_count == 0)
            return &incore[i];
    }

    return NULL;
}

struct inode *find_incore(unsigned int inode_num) {

    for(int i = 0; i < MAX_SYS_OPEN_FILES; i++) {

        if(incore[i].ref_count != 0 && incore[i].inode_num == inode_num) {
            return &incore[i];
        }
    }

    return NULL;
}

void read_inode(struct inode *in, int inode_num) {

    int block_num = inode_num / INODES_PER_BLOCK + INODE_FIRST_BLOCK;
    int block_offset = inode_num % INODES_PER_BLOCK;  // offset in inodes
    int block_offset_bytes = block_offset * INODE_SIZE;

    unsigned char *block;
    block = bread(block_num, block);

    in -> size = read_u32(block + block_offset_bytes + INODE_SIZE_OFFSET); 
    in -> owner_id = read_u16(block + block_offset_bytes + INODE_OWNERID_OFFSET);
    in -> permissions = read_u8(block + block_offset_bytes + INODE_PERMISSIONS_OFFSET);
    in -> flags = read_u8(block + block_offset_bytes + INODE_FLAGS_OFFSET);
    in -> link_count = read_u8(block + block_offset_bytes + INODE_LINK_COUNT_OFFSET);
    in -> block_ptr[INODE_PTR_COUNT] = read_u16(block + block_offset_bytes + INODE_BLOCK_PTR_OFFSET);

}

void write_inode(struct inode *in) {

    int inode_num = in -> inode_num;
    int block_num = inode_num / INODES_PER_BLOCK + INODE_FIRST_BLOCK;
    int block_offset = inode_num % INODES_PER_BLOCK;  // offset in inodes
    int block_offset_bytes = block_offset * INODE_SIZE;
    
    unsigned char *block;
    block = bread(block_num, block);

    write_u32(block + block_offset_bytes + INODE_SIZE_OFFSET, in -> size);
    write_u16(block + block_offset_bytes + INODE_OWNERID_OFFSET, in -> owner_id);
    write_u8(block + block_offset_bytes + INODE_PERMISSIONS_OFFSET, in -> permissions);
    write_u8(block + block_offset_bytes + INODE_FLAGS_OFFSET, in -> flags);
    write_u8(block + block_offset_bytes + INODE_LINK_COUNT_OFFSET, in -> link_count);
    write_u16(block + block_offset_bytes + INODE_BLOCK_PTR_OFFSET, in -> block_ptr[INODE_PTR_COUNT]);

    block = bread(block_num, block);
    bwrite(block_num, block);
}

struct inode *iget(int inode_num) {




}

void iput(struct inode *in) {


}

