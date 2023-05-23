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


void alloc_test1(void) {

    int test_alloc = alloc();

    CTEST_ASSERT((test_alloc == 7), "Testing allocation of first 7 blocks after running mkfs()");
    image_fd = image_close();
}


void alloc_test2(void) {

    image_fd =image_open("test", 1);
    int alloc1 = alloc();
    int alloc2 = alloc();

    CTEST_ASSERT((alloc1 != alloc2), "Multiple calls to alloc() return different free blocks");
    image_fd = image_close();
}

void ialloc_test(void) {

    image_fd = image_open("test", 1);
    int ialloc1 = ialloc();
    int ialloc2 = ialloc();

    CTEST_ASSERT((ialloc1 != ialloc2), "Multiple calls to ialloc() return different inode blocks");
    image_fd = image_close();
}

int main(void) {

    CTEST_VERBOSE(1);

    image_fd = image_open("test", 1);

    mkfs();

    alloc_test1();
    
    alloc_test2();
    
    ialloc_test();
    

    
    
}