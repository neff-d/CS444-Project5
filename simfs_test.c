#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "image.h"
#include "mkfs.h"
#include "inode.h"
#include "block.h"
#include "free.h"
#include "ctest.h"
#include "pack.h"
#include "directory.h"
#include "ls.h"
#include "dirbasename.h"


void setup(void) {

    image_fd = image_open("test", 1);
    mkfs();
}

void teardown(void) {

    image_fd = image_close();
    clear_incore();
}

void image_open_test(void) {

    setup();

    CTEST_ASSERT(image_fd >= 0, "image_open() returns a valid file descriptor");

    teardown();
}

void image_close_test(void) {

    setup();

    CTEST_ASSERT(image_fd = image_close() == 0, "image_close() is successful");
}

void alloc_test(void) {

    setup();

    int test_alloc = alloc();

    CTEST_ASSERT(test_alloc == 8, "Testing allocation of first 8 blocks after running mkfs()");

    int alloc1 = alloc();
    int alloc2 = alloc();

    CTEST_ASSERT(alloc1 != alloc2, "Multiple calls to alloc() return different free blocks");
    
    teardown();
}

void ialloc_test(void) {

    setup();

    struct inode *ialloc1 = ialloc();
    struct inode *ialloc2 = ialloc();

    CTEST_ASSERT(ialloc1 -> inode_num != ialloc2 -> inode_num, "Multiple calls to ialloc() return different inode nums");
    CTEST_ASSERT(ialloc1 -> inode_num == 1, "First call to ialloc() == 1");
    CTEST_ASSERT(ialloc2 -> inode_num == 2, "Second call to ialloc() == 2");

    set_incore();

    struct inode *alloc3 = ialloc();

    CTEST_ASSERT(alloc3 == NULL, "ialloc() returns NULL if there are no free inodes");
    
    teardown();
}

void set_free_test(void) {

    setup();

    unsigned char test_block[BLOCK_SIZE] = {0xFE, 0x7F}; // 1111 1110, 0111 1111
    int test_bit_num = 0;
    int bit1 = find_free(test_block);

    set_free(test_block, test_bit_num, SET);

    CTEST_ASSERT(find_free(test_block) != 0, " Expect bit 0 is no longer free");

    int bit2 = find_free(test_block);

    CTEST_ASSERT(bit2 == 15, "bit2 returns 15 when test_block[0] is full and test_block[1] = 0x7F");
    CTEST_ASSERT(bit1 != bit2, "find_free() returns a different bit after bit1 is set");

    set_free(test_block, test_bit_num, CLEAR);

    bit1 = find_free(test_block);
    
    CTEST_ASSERT(bit1 == 0, "find_free() returns bit 0 after calling set_free() with CLEAR");

    set_free(test_block, test_bit_num, SET);

    test_bit_num = 15;

    set_free(test_block, test_bit_num, SET);

    int bit3 = find_free(test_block);

    CTEST_ASSERT(find_free(test_block) == 16, "Expect next free bit is 16 after setting bit 15");

    memset(test_block, 0xFF, BLOCK_SIZE);

    CTEST_ASSERT(find_free(test_block) == -1, "Expect no free bits to be returned");


    teardown();
}

void find_free_test(void) {

    setup();

    unsigned char test_block[BLOCK_SIZE] = {0x8F}; // 1000 1111

    CTEST_ASSERT(find_free(test_block) == 4, "Expect first free bit == 4 when test_block[0] == 1000 1111");

    set_free(test_block, 4, SET);

    CTEST_ASSERT(find_free(test_block) == 5, "Expect next free bit == 5 after setting bit 4");

    teardown();
}

void bread_test(void) {

    setup();

    unsigned char test_block1[BLOCK_SIZE];
    int test_block_num1 = NUM_BLOCKS - 1;
    unsigned char *return_block1 = bread(test_block_num1, test_block1);
    
    CTEST_ASSERT(return_block1 != NULL, "Returns a valid block");

    unsigned char test_block2[BLOCK_SIZE];
    int test_block_num2 = -1;
    unsigned char *return_block2 = bread(test_block_num2, test_block2);

    CTEST_ASSERT(return_block2 == NULL, "Returns NULL when given a negative block number");

    unsigned char test_block3[BLOCK_SIZE];
    int test_block_num3 = NUM_BLOCKS;
    unsigned char *return_block3 = bread(test_block_num3, test_block3);

    CTEST_ASSERT(return_block3 == NULL, "Returns NULL when given a block beyond the scope of the file");
    
    teardown();
}

void bwrite_test(void) {

    setup();

    unsigned char test_block1[BLOCK_SIZE] = {0x3F, 0x5D, 0xB7};
    unsigned char test_block2[BLOCK_SIZE] = {0xC2, 0xD8, 0xBD};
    int test_block_num1 = 8;
    int test_block_num2 = 12;

    bwrite(test_block_num1, test_block1);
    bwrite(test_block_num2, test_block2);

    unsigned char *return_block1 = bread(test_block_num1, test_block1);
    unsigned char *return_block2 = bread(test_block_num2, test_block2);

    CTEST_ASSERT(memcmp(return_block1, return_block2, BLOCK_SIZE) != 0, "bwrite() writes blocks with different bit values");

    unsigned char test_block3[BLOCK_SIZE] = {0xF8, 0x6B, 0xCC};
    unsigned char test_block4[BLOCK_SIZE] = {0xF8, 0x6B, 0xCC};
    test_block_num1 = 16;
    test_block_num2 = 48;

    bwrite(test_block_num1, test_block3);
    bwrite(test_block_num2, test_block4);

    return_block1 = bread(test_block_num1, test_block3);
    return_block2 = bread(test_block_num2, test_block4);

    CTEST_ASSERT(memcmp(return_block1, return_block2, BLOCK_SIZE) == 0, "bwrite() writes the same data to different blocks");

    teardown();
}

void find_incore_free_test(void) {

    setup();

    struct inode *in = find_incore_free();
    set_nth_incore(0);

    CTEST_ASSERT(in -> inode_num == 0, "find_incore_free() returns inode 0 when the incore array is empty");

    struct inode *in1 = find_incore_free();
    set_nth_incore(2);
    
    CTEST_ASSERT(in1 -> inode_num == 2, "find_incore_free() returns inode 2 to be returned when inode 1 is taken");

    set_incore();
    clear_nth_incore(47);

    struct inode *in3 = find_incore_free();
    set_nth_incore(47);

    CTEST_ASSERT(in3 != NULL, "find_incore_free() returns inode 47 to be returned when it is the only free inode");

    struct inode *in4 = find_incore_free();

    CTEST_ASSERT(in4 == NULL, "find_incore_free() returns NULL when there are no free inodes");

    teardown();
}

void find_incore_test(void) {

    setup();

    struct inode *in1 = ialloc();
    struct inode *in2 = ialloc();

    struct inode *test1 = find_incore(in1 -> inode_num);
    struct inode *test2 = find_incore(in2 -> inode_num);

    CTEST_ASSERT(test1 == in1, "find_incore() returns correct pointer based on inode number");
    CTEST_ASSERT(test2 != test1, "find_incore() returns different pointers for different inode numbers");

    struct inode *test3 = find_incore(6);

    CTEST_ASSERT(test3 == NULL, "find_incore() returns NULL given an inode number where its reference count = 0");

    teardown();
}

void read_write_inode_test(void) {

    setup();

    struct inode *in1 = ialloc();
    struct inode *in2 = ialloc();
    
    in1 -> size = 48;
    in1 -> owner_id = 4;
    in1 -> permissions = 'W';
    in1 -> flags = 'F';
    in1 -> link_count = '3';
    in1 -> block_ptr[4] = 2;

    write_inode(in1);
    read_inode(in2, in1 -> inode_num);
    
    CTEST_ASSERT(in2 -> size == in1 -> size, "read_inode() returns correct size");
    CTEST_ASSERT(in2 -> owner_id == in1 -> owner_id, "read_inode() returns correct owner_id");
    CTEST_ASSERT(in2 -> permissions == in1 -> permissions, "read_inode() returns correct permissions");
    CTEST_ASSERT(in2 -> flags == in1 -> flags, "read_inode() returns correct flags");
    CTEST_ASSERT(in2 -> link_count == in1 -> link_count, "read_inode() returns correct link_count");
    CTEST_ASSERT(in2 -> block_ptr[4] == in1 -> block_ptr[4], "read_inode returns correct block pointer");

    struct inode *in3 = ialloc();
    struct inode *in4 = ialloc();

    in3 -> inode_num = 255;
    in3 -> size = 63;
    in3 -> owner_id = 2;
    in3 -> permissions = 'R';
    in3 -> flags = 'C';
    in3 -> link_count = '4';
    in3 -> block_ptr[15] = 1;

    write_inode(in3);
    read_inode(in4, in3 -> inode_num);

    CTEST_ASSERT(in4 -> size == in3 -> size, "read_inode() returns correct size");
    CTEST_ASSERT(in4 -> owner_id == in3 -> owner_id, "read_inode() returns correct owner_id");
    CTEST_ASSERT(in4 -> permissions == in3 -> permissions, "read_inode() returns correct permissions");
    CTEST_ASSERT(in4 -> flags == in3 -> flags, "read_inode() returns correct flags");
    CTEST_ASSERT(in4 -> link_count == in3 -> link_count, "read_inode() returns correct link_count");
    CTEST_ASSERT(in4 -> block_ptr[15] == in3 -> block_ptr[15], "read_inode returns correct block_ptr");

    struct inode *in5 = ialloc();
    struct inode *in6 = ialloc();

    in5 -> inode_num = 76;
    in5 -> size = 33;
    in5 -> owner_id = 1;
    in5 -> permissions = 'R';
    in5 -> flags = 'A';
    in5 -> link_count = '5';
    in5 -> block_ptr[11] = 3;

    write_inode(in5);
    read_inode(in6, in5 -> inode_num);

    CTEST_ASSERT(in6 -> size == in5 -> size, "read_inode() returns correct size");
    CTEST_ASSERT(in6 -> owner_id == in5 -> owner_id, "read_inode() returns correct owner_id");
    CTEST_ASSERT(in6 -> permissions == in5 -> permissions, "read_inode() returns correct permissions");
    CTEST_ASSERT(in6 -> flags == in5 -> flags, "read_inode() returns correct flags");
    CTEST_ASSERT(in6 -> link_count == in5 -> link_count, "read_inode() returns correct link_count");
    CTEST_ASSERT(in6 -> block_ptr[11] == in5 -> block_ptr[11], "read_inode returns correct block_ptr");


    teardown();
}

void iget_test(void) {

    setup();

    struct inode *in1 = ialloc();
    
    struct inode *in2 = iget(in1 -> inode_num);

    CTEST_ASSERT(in1 == in2, "iget() returns a pointer to an existing inode");
    CTEST_ASSERT(in1 -> ref_count == 2, "iget() increments reference count when returning an existing in-core inode");

    struct inode *in3 = iget(7);

    CTEST_ASSERT(in3 -> inode_num == 7, "iget() sets inode_num to the value passed in if that in-core inode is free");
    CTEST_ASSERT(in3 -> ref_count == 1, "iget() sets inode reference count to 1 when returning a free in-core inode pointer");

    set_incore();

    struct inode *in4 = iget(256);

    CTEST_ASSERT(in4 == NULL, "iget() returns NULL when given an inode number out of range");

    teardown();
}

void iput_test(void) {

    setup();

    struct inode *in1 = ialloc();

    in1 -> ref_count = 2;

    iput(in1);
    read_inode(in1, in1 -> inode_num);

    CTEST_ASSERT(in1 -> ref_count == 1, "iput() decrements reference count of an existing inode");

    struct inode *in2 = ialloc();

    iput(in2);
    read_inode(in2, in2 -> inode_num);

    struct inode *in3 = find_incore(in2 -> inode_num);

    CTEST_ASSERT(in3 == NULL, "iput() frees an inode if reference count = 0");

    teardown();
}

void directory_open_test(void) {

    setup();

    set_incore();

    int inode_num = 1;

    struct directory *dir1 = directory_open(inode_num);
    
    CTEST_ASSERT(dir1 == NULL, "directory_open() returns NULL when all incore inodes are set");

    teardown();

    setup();

    inode_num = 2;

    struct directory *dir2 = directory_open(inode_num);

    CTEST_ASSERT(dir2 != NULL, "directory_open() returns pointer to a struct directory when incore array is not full");
    CTEST_ASSERT(dir2 -> inode == iget(inode_num), "directory_open() returns a directory with the inode of the same inode_num that was passed in");
    CTEST_ASSERT(dir2 -> inode -> inode_num == inode_num, "'directory_open() returns a directory whose inode_num is the same as what was passed in");
    CTEST_ASSERT(dir2 -> offset == DIR_BASE_OFFSET, "directory_open() initializes its offset to the base offset of 0");

    directory_close(dir2);

    teardown();
}

void directory_get_test(void) {

    setup();

    int inode_num1 = 4;
    struct directory_entry *ent1 = malloc(sizeof(struct directory_entry));
    struct directory *dir1 = directory_open(inode_num1);
    dir1 -> offset = 4097;

    CTEST_ASSERT(directory_get(dir1, ent1) == -1, "directory_get() returns -1 when dir1 -> offset is greater than dir1 -> inode -> size");

    dir1 -> offset = DIR_BASE_OFFSET;

    CTEST_ASSERT(directory_get(dir1, ent1) == 0, "directory_get() returns 0 on success");

    free(ent1);
    directory_close(dir1); 
    teardown();

    setup();

    struct directory_entry *ent2 = malloc(sizeof(struct directory_entry));
    struct directory_entry *ent3 = malloc(sizeof(struct directory_entry));
    struct directory *dir2 = directory_open(0);

    int result = directory_get(dir2, ent2);
    result = directory_get(dir2, ent3);

    CTEST_ASSERT(strcmp(ent2 -> name, ".") == 0, "directory_get() returns the first entry's name on the first call to it");
    CTEST_ASSERT(strcmp(ent3 -> name, "..") == 0, "directory_get() properly increments offset in the directory before returning");

    free(ent2);
    free(ent3);
    directory_close(dir2);

    set_nth_incore(0);
    struct directory_entry *ent4 = malloc(sizeof(struct directory_entry));
    struct directory *dir3 = directory_open(85);

    result = directory_get(dir3, ent4);

    CTEST_ASSERT(strcmp(ent4 -> name, ".") != 0, "directory_get() doesn't read an entry name that doesn't exist");

    free(ent4);
    directory_close(dir3);
    teardown();
}

void directory_close_test(void) {

    setup();

    struct directory *dir1 = directory_open(98);
    int dir1_inode_num = dir1 -> inode -> inode_num;

    directory_close(dir1);

    struct inode *in1 = find_incore(dir1_inode_num);

    CTEST_ASSERT(in1 == NULL, "directory_close() frees the directory's inode when its reference count = 0");

    teardown();
}

void namei_test(void) {

    setup();

    struct inode *in1 = namei("/");

    CTEST_ASSERT(in1 != NULL, "namei() returns an inode when called on root directory");
    CTEST_ASSERT(in1 -> inode_num == 0, "namei() returns root directory's inode when called with '/'");

    struct inode *in2 = namei("/foo");

    CTEST_ASSERT(in2 == NULL, "namei() returns NULL when given a path that does not exist");

    teardown();
}

void directory_make_test(void) {

    setup();

    set_incore();

    int result = directory_make("/foo");

    CTEST_ASSERT(result == -1, "directory_make() returns failed when the incore array is full");

    char buffer[BLOCK_SIZE];

    bread(BLOCK_MAP_NUM, buffer);
    memset(buffer, 0xFF, BLOCK_SIZE);
    bwrite(BLOCK_MAP_NUM, buffer);

    result = directory_make("/bar");

    CTEST_ASSERT(result == -1, "directory_make() returns failed when all data blocks are full");

    teardown();

    setup();

    result = directory_make("/baz");

    struct directory *dir1 = directory_open(1);
    struct directory_entry *ent1 = malloc(sizeof(struct directory_entry));

    directory_get(dir1, ent1);
    directory_get(dir1, ent1);
    directory_get(dir1, ent1);

   
    CTEST_ASSERT(strcmp(ent1 -> name, "baz") == 0, "directory_make() adds correct director name");
    CTEST_ASSERT(ent1 -> inode_num == dir1 -> inode -> inode_num, "directory_make() sets the directory's inode number to its entry inode number in root directory ");
    CTEST_ASSERT(result == 0, "directory_make() returns 0 when successful");

    free(ent1);
    teardown();
}

void make_some_directories(void) {

    setup();

    printf("Making some directories...\n");

    directory_make("/foo");
    directory_make("/bar");
    directory_make("/baz");
    directory_make("/frobozz");
    directory_make("/yabba");
    directory_make("/dabba");
    directory_make("/doo");

    ls(0);

    teardown();
}



int main(void) {

    CTEST_VERBOSE(1);
    CTEST_COLOR(1);

    image_open_test();

    image_close_test();

    alloc_test();
    
    ialloc_test();

    bread_test();
    
    set_free_test();
    
    find_free_test();

    bwrite_test();

    find_incore_free_test();

    find_incore_test();

    read_write_inode_test();

    iget_test();

    iput_test();

    directory_open_test();

    directory_get_test();

    directory_close_test();

    namei_test();

    directory_make_test();

    make_some_directories();

    CTEST_RESULTS();
    
}