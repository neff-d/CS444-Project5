#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "mkfs.h"
#include "image.h"
#include "inode.h"
#include "free.h"
#include "block.h"

void mkfs(void) {

    unsigned char buffer[4194304] = {0};

    write(image_fd, buffer, sizeof buffer);
    if(image_fd == -1)
        perror("write");


    for(int i = 0; i < 7; i++) {
        alloc();
    }
    printf("Write and alloc complete\n");
}