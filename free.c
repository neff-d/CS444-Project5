#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "free.h"
#include "block.h"

// Do these first

void set_free(unsigned char *block, int num, int set) {

    int byte_num = num / 8;
    int bit_num = num % 8;

    if(set == 1) {
        block[byte_num] |= 1 << bit_num;
    }

    else {
        block[byte_num] &= ~(1 << bit_num);
    }

    // bwrite() happens outside of whatever calls set_free()
}


int find_free(unsigned char *block) {

    for(int i = 0; i < BLOCK_SIZE; i++) {

        int free_bit = find_low_clear_bit(block[i]);
        if(free_bit != -1)
            return (i * 8) + free_bit;
    }
    return -1;    
}


int find_low_clear_bit(unsigned char x)
{
    for (int i = 0; i < 8; i++)
        if (!(x & (1 << i)))
            return i;
    
    return -1;
}