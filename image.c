#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "image.h"

int image_fd;

int image_open(char *filename, int truncate) {

    image_fd = open("output.txt", O_RDWR | O_CREAT | O_TRUNC, 0600);
}

int image_close(void) {
    
    close(image_fd);
}