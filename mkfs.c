#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "mkfs.h"
#include "image.h"

void mkfs(void) {

    unsigned char buffer[4194304] = {0};

    write(image_fd, buffer, sizeof buffer);
    if(image_fd == -1)
        perror("write");


    // use alloc() to set the first 7 blocks to 1


}