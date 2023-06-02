#include <stdlib.h>
#include "inode.h"
#include "block.h"
#include "free.h"
#include "pack.h"

static struct inode incore[MAX_SYS_OPEN_FILES] = {0};

struct inode *ialloc(void) {

    unsigned char block[BLOCK_SIZE];
    unsigned char *inode_block = bread(INODE_MAP_NUM, block);

    int free_block_num = find_free(inode_block);

    if(free_block_num == -1)
        return NULL;
    
    set_free(inode_block, free_block_num, SET);
    bwrite(INODE_MAP_NUM, inode_block);

    struct inode *in;

    if((in = iget(free_block_num)) == NULL)
        return NULL;

    in -> size, in -> owner_id, in -> permissions, in -> flags = 0, 0, 0, 0;
    in -> inode_num = free_block_num;

    for(int i = 0; i < INODE_PTR_COUNT; i++) {
        in -> block_ptr[i] = 0;
    }

    write_inode(in);
    
    return in;
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
}

struct inode *iget(int inode_num) {

    struct inode *in;

    if((in = find_incore(inode_num)) != NULL) {
        in -> ref_count++;
        return in;
    }

    if((in = find_incore_free()) == NULL)
        return NULL;

    read_inode(in, in -> inode_num);
    in -> ref_count = 1;
    in -> inode_num = inode_num;

    return in;
}

void iput(struct inode *in) {

    if(in -> ref_count == 0)
        return;

    in -> ref_count--;

    if(in -> ref_count == 0)
        write_inode(in);
}

