#include "utils.h"
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/stat.h>

#define SIZE 200

int file_exists(char *path)
{
    FILE *file;
    file = fopen(path, "r");
    if (file)
    {
        fclose(file);
        return 1;
    }
    else
        return 0;
}

void validate_user(int sockfd, char *username)
{
    DIR *dir = opendir(username);
    if (dir)
        closedir(dir);
    else if (ENOENT == errno)
    {
        if (mkdir(username, 0777))
        {
            perror("Error creating folder");
            close(sockfd);
            exit(1);
        }
    }
    else
    {
        perror("Error opening folder");
        close(sockfd);
        return;
    }
}

void handle_file_receiving(int sockfd, char *username)
{
    char filename[SIZE] = {0};
    char path[SIZE] = {0};
    if (recv(sockfd, filename, SIZE, 0) <= 0)
    {
        perror("Error receiving filename from client");
        close(sockfd);
        return;
    }
    create_file_path(path, username, filename);
    if (file_exists(path))
    {
        send(sockfd, "bad", strlen("bad"), 0);
        perror("file already exists");
        close(sockfd);
        return;
    }
    else if (send(sockfd, "good", strlen("good"), 0) == -1)
    {
        perror("Error sending response to client");
        close(sockfd);
        return;
    }
    write_file(username, filename, sockfd, 1);
    return;
}

void handle_file_sending(int sockfd, char *username)
{
    char filename[SIZE] = {0};
    char path[SIZE] = {0};
    if (recv(sockfd, filename, SIZE, 0) <= 0)
    {
        perror("Error receiving filename from client");
        close(sockfd);
        return;
    }
    create_file_path(path, username, filename);
    if (!file_exists(path))
    {
        send(sockfd, "bad", strlen("bad"), 0);
        perror("file does not exists");
        close(sockfd);
        return;
    }
    if (send(sockfd, "good", strlen("good"), 0) == -1)
    {
        perror("Error sending response to client");
        close(sockfd);
        return;
    }
    send_file(username, filename, sockfd,1);
    return;
}

void handle_connection(int sockfd)
{
    char selection;
    char username[SIZE] = {0};

    if (recv(sockfd, username, SIZE, 0) <= 0)
    {
        perror("Error receiving username from client");
        close(sockfd);
        return;
    }
    validate_user(sockfd, username);
    if (recv(sockfd, &selection, sizeof(selection), 0) <= 0)
    {
        perror("Error receiving data from client");
        close(sockfd);
        return;
    }
    switch (selection)
    {
        case '1':
            handle_file_receiving(sockfd, username);
            break;
        case '2':
            handle_file_sending(sockfd, username);
            break;
        default:
            puts("client sent wrong option");
            close(sockfd);
            return;
    }
    close(sockfd);
}

int main()
{
    char *ip = "127.0.0.1";
    int port = 8080;
    int e;

    int server_socket;
    fd_set current_sockets, ready_sockets;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[SIZE];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("Error in socket");
        exit(1);
    }
    printf("Server socket created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    e = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (e < 0)
    {
        perror("Error in bind");
        exit(1);
    }

    if (listen(server_socket, 10) == 0)
        printf("Listening....\n");
    else
    {
        perror("Error in listening");
        exit(1);
    }

    FD_ZERO(&current_sockets);
    FD_SET(server_socket, &current_sockets);

    addr_size = sizeof(new_addr);
    while (1)
    {
        ready_sockets = current_sockets;

        if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
        {
            perror("Error with select");
            close(server_socket);
            exit(1);
        }

        for (int i = 0; i < FD_SETSIZE; i++)
            if (FD_ISSET(i, &ready_sockets))
            {
                if (i == server_socket)
                {
                    int client_socket = accept(server_socket, (struct sockaddr *)&new_addr, &addr_size);
                    FD_SET(client_socket, &current_sockets);
                }
                else
                {
                    handle_connection(i);
                    FD_CLR(i, &current_sockets);
                }
            }
    }

    return 0;
}
