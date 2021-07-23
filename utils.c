
#include <stdio.h>
#include "utils.h"

#define SIZE 200

void send_file(FILE *fp, int sockfd){
    int n;
    char data[SIZE] = {0};

    while(fgets(data, SIZE, fp) != NULL) {
        if (send(sockfd, data, sizeof(data), 0) == -1) {
            perror("[-]Error in sending file.");
            exit(1);
        }
        bzero(data, SIZE);
    }
}

void write_file(int sockfd, char * filename){
    int n;
    FILE *fp;
    char buffer[SIZE];

    fp = fopen(filename, "w");
    while (recv(sockfd, buffer, SIZE, 0) > 0) {
        fprintf(fp, "%s", buffer);
        bzero(buffer, SIZE);
    }
    return;
}
