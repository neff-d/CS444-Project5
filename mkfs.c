#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "mkfs.h"
#include "image.h"

void mkfs(void) {

unsigned char buffer[4194304];

for(int i = 0; i < 4194304; i++) {
    buffer[i] = '0';
}

for(int i = 0; i < 7; i++) {
    buffer[i] = '1';
}

for(int i = 0; i < 20; i++) {
    printf("buffer[%d] = %c\n", i, buffer[i]);
}

write(image_fd, buffer, sizeof buffer);
if(image_fd == -1)
    perror("write");

// printf("image_fd[23] = %d\n", image_fd[23]);

// mark all of the important blocks as allocated
// opens the file
// create the array here
// initialize all of the bytes to 0


//close image

}