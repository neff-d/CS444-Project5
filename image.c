#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "image.h"

int image_fd;

int image_open(char *filename, int truncate) {

puts (filename);

    if(truncate) 
        image_fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0600);

    else    
        image_fd = open(filename, O_RDWR | O_CREAT, 0600);

    return image_fd;
}

int image_close(void) {
    
    close(image_fd);
}
