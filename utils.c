
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

#define SIZE 200



void create_file_path(char * path, char * username, char * filename)
{
    strcat(path, username);
    strcat(path, "/");
    strcat(path, filename);
}


void send_file(char * username, char * filename, int sockfd, int flag){
    int n;
    char data[SIZE] = {0};
    char file_path[SIZE] = "";

    if (flag)
        create_file_path(file_path, username, filename);
    else
        strcpy(file_path, filename);
    FILE * fp = fopen(file_path, "r");

    if (fp == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    while(fgets(data, SIZE, fp) != NULL) {
        if (send(sockfd, data, sizeof(data), 0) == -1) {
            perror("Error in sending file.");
            exit(1);
        }
        bzero(data, SIZE);
    }
    return;
}

void write_file(char * username, char * filename, int sockfd, int flag){
    int n;
    FILE *fp;
    char buffer[SIZE] = {0};
    char file_path[SIZE] = {0};

    if (flag)
        create_file_path(file_path, username, filename);
    else
        strcpy(file_path, filename);
    fp = fopen(file_path, "w");

    if (fp == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    while (recv(sockfd, buffer, SIZE, 0) > 0) {
        fprintf(fp, "%s", buffer);
        fflush(fp);
        bzero(buffer, SIZE);
    }
    fflush(fp);
    return;
}

void error_exit(char * error, int exit_code)
{
    perror(error);
    exit(exit_code);
}
