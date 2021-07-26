#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SIZE 200
#define IP "127.0.0.1"
#define PORT 8080


void print_menu()
{
    puts("Menu - enter the number of your desired option :");
    puts("1) upload file");
    puts("2) download file");
}

void handle_file_upload(int sockfd, char * username)
{
    char filename[SIZE] = {0};
    char server_answer[SIZE] = {0};
    puts("please enter the name of the file you want to upload");
    scanf("%s", filename);

    if (send(sockfd, filename, SIZE, 0) == -1)
        error_exit("Error sending data to server", 1);

    if (recv(sockfd, server_answer, SIZE, 0) <= 0)
        error_exit("Error receiving data from server", 1);

    if (strcmp(server_answer, "bad") == 0)
    {
        puts("file already exists");
        return;
    }
    send_file(username, filename, sockfd, 0);
}

void handle_file_download(int sockfd, char * username)
{
    char filename[SIZE] = {0};
    char server_answer[10] = {0};
    puts("please enter the name of the file you want to download");
    scanf("%s", filename);

    if (send(sockfd, filename, SIZE, 0) == -1)
        error_exit("Error sending data to server", 1);

    if (recv(sockfd, server_answer, 10, 0) <= 0)
        error_exit("Error receiving data from server", 1);
        
    if (strcmp(server_answer, "bad") == 0)
    {
        puts("file does not exists");
        return;
    }
    write_file(username, filename, sockfd, 0);
}

void send_selection_to_server(int sockfd, int selection)
{
    char sel = selection + '0';
    if (send(sockfd, &sel, sizeof(sel), 0) == -1)
        error_exit("Error sending data to server", 1);
    return;
}

void client_menu()
{
    int answer = 0;
    int sockfd;
    struct sockaddr_in server_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    char username[SIZE] = {0};

    if(sockfd < 0) 
        error_exit("Error in socket", 1);
    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(IP);
    server_addr.sin_port = htons(PORT);

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0)
        error_exit("Error in connect", 1);
    puts("Connected to server!");

    puts("please enter username : ");
    scanf("%s", username);
    if (send(sockfd, username, strlen(username), 0) == -1)
        error_exit("Error sending data to server", 1);

    print_menu();
    scanf("%d", &answer);
    switch (answer)
    {
        case 1:
            send_selection_to_server(sockfd, 1);
            handle_file_upload(sockfd, username);
            break;
        case 2:
            send_selection_to_server(sockfd, 2);
            handle_file_download(sockfd, username);
            break;
        default:
            puts("wrong input. please try again");
    }
    close(sockfd);
}

int main()
{
    client_menu();
    return 0;
}