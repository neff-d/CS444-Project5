#include <stdio.h>
#include "ls.h"
#include "mkfs.h"
#include "inode.h"
#include "directory.h"

void ls(int dir_num)
{
    struct directory *dir;
    struct directory_entry ent;

    dir = directory_open(0);

    while (directory_get(dir, &ent) != -1)
        printf("%d %s\n", ent.inode_num, ent.name);

    directory_close(dir);
}