#ifndef FREE_H
#define FREE_H

#define SET                 1
#define CLEAR               0
#define MAX_BIT_NUM     32767

void set_free(unsigned char *block, int num, int set);
int find_free(unsigned char *block);
int find_low_clear_bit(unsigned char x);

#endif