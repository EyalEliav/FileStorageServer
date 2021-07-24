#include "utils.h"
#include <dirent.h>
#include <errno.h>
#include <string.h>

#define SIZE 200

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

void handle_file_receiving(int sockfd, char * username)
{
    //TODO
}

void handle_file_sending(int sockfd, char * username)
{
    //TODO
}

void handle_connection(int sockfd)
{
    //TODO - add receiving username
    char selection;
    if (recv(sockfd, selection, sizeof(selection), 0) <= 0)
    {
        perror("Error receiving data from client");
        close(sockfd);
        return;
    }
    switch (selection)
    {
        case '1':

    }
}


int main()
{
    return 0;
}
