
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

#define SIZE 200

void exit_prog(char * error, int exit_code)
{
    perror(error);
    exit(exit_code);
}

char * create_file_path(char * username, char * filename)
{
    char file_path[SIZE] = "";
    strcat(file_path, username);
    strcat(file_path, "/");
    strcat(file_path, filename);
    return file_path;
}


void send_file(char * username, char * filename, int sockfd){
    int n;
    char data[SIZE] = {0};
    char * file_path = create_file_path(username, filename);
    FILE * fp = fopen(file_path, "r");

    if (fp == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    while(fgets(data, SIZE, fp) != NULL) {
        if (send(sockfd, data, sizeof(data), 0) == -1) {
            perror("[-]Error in sending file.");
            exit(1);
        }
        bzero(data, SIZE);
    }
    return;
}

void write_file(char * username, char * filename, int sockfd){
    int n;
    FILE *fp;
    char buffer[SIZE];
    char * file_path = create_file_path(username, filename);
    fp = fopen(file_path, "w");

    if (fp == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    while (recv(sockfd, buffer, SIZE, 0) > 0) {
        fprintf(fp, "%s", buffer);
        bzero(buffer, SIZE);
    }
    return;
}
