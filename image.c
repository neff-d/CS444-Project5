#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "image.h"

int image_fd;

int image_open(char *filename, int truncate) {

    if(!truncate) {
        image_fd = open("output.txt", O_RDWR | O_CREAT, 0600);
    }
    else    
        image_fd = open("output.txt", O_RDWR | O_CREAT | O_TRUNC, 0600);

    return image_fd;
}

int image_close(void) {
    
    close(image_fd);
}
