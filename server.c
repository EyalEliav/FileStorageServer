#include "utils.h"
#include <dirent.h>
#include <errno.h>
#include <string.h>

int validate_user(char *username)
{
    DIR *dir = opendir(username);
    if (dir)
    {
        closedir(dir);
        return 1;
    }
    else if (ENOENT == errno)
        return 0;
    else
        return -1;
}


int main()
{
    return 0;
}
