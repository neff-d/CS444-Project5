#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "image.h"
#include "mkfs.h"
#include "ctest.h"


int main(void) {

    char *myFile = "output";

    // call image_open first

    image_fd = image_open(myFile, 1);

    // call mkfs to write the huge buffer into the file descriptor

    mkfs();

    










    image_close();
    
}