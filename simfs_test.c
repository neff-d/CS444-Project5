#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "image.h"
#include "ctest.h"


int main(void) {

    char *myFile;

    // call image_open first
    image_open(*myFile, 1);

    // call mkfs to write the huge buffer into the file descriptor











    image_close();
    
}