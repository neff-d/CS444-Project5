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


void setup(void) {

    image_fd = image_open("test", 1);
    mkfs();
}

void teardown(void) {

    image_fd = image_close();
}

void alloc_test(void) {

    setup();

    int test_alloc = alloc();

    CTEST_ASSERT((test_alloc == 7), "Testing allocation of first 7 blocks after running mkfs()");

    int alloc1 = alloc();
    int alloc2 = alloc();

    CTEST_ASSERT((alloc1 != alloc2), "Multiple calls to alloc() return different free blocks");
    
    teardown();
}

void ialloc_test(void) {

    setup();

    int ialloc1 = ialloc();
    int ialloc2 = ialloc();

    CTEST_ASSERT((ialloc1 != ialloc2), "Multiple calls to ialloc() return different inode blocks");
    CTEST_ASSERT((ialloc1 == 0), "First call to ialloc() == 0");
    CTEST_ASSERT((ialloc2 == 1), "Second call to ialloc() == 1");
    
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

    CTEST_ASSERT(find_free(test_block) == 4, "Expect first free bit == 4");

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

int main(void) {

    CTEST_VERBOSE(1);


    alloc_test();
    
    ialloc_test();

    bread_test();
    
    set_free_test();
    
    find_free_test();
    
}