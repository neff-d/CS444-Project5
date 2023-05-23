#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

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

void alloc_test1(void) {

    setup();

    int test_alloc = alloc();

    CTEST_ASSERT((test_alloc == 7), "Testing allocation of first 7 blocks after running mkfs()");
    
    teardown();
}


void alloc_test2(void) {

    setup();

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

    unsigned char test_block[BLOCK_SIZE];
    int test_block_num = 12;

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

    CTEST_ASSERT(return_block2 == NULL, "Fails to return a negative block number");

    unsigned char test_block3[BLOCK_SIZE];
    int test_block_num3 = NUM_BLOCKS;
    unsigned char *return_block3 = bread(test_block_num3, test_block3);

    CTEST_ASSERT(return_block3 == NULL, "Fails to return a block beyond the scope of the file");
    
    teardown();
}

int main(void) {

    CTEST_VERBOSE(1);


    alloc_test1();
    
    alloc_test2();
    
    ialloc_test();

    bread_test();
    

    
    
}