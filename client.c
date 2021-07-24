#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SIZE 200
#define IP "127.0.0.1"
#define PORT 8080;


void print_menu()
{
    puts("Menu - enter the number of your desired option :");
    puts("1) upload file");
    puts("2) download file");
    puts("3) exit");
}

void handle_file_upload(int sockfd, char * username)
{
    char filename[SIZE] = {0};
    char server_answer[10] = {0};
    puts("please enter the name of the file you want to upload");
    gets(filename);

    if (send(sockfd, filename, strlen(username), 0) == -1)
        exit_prog("Error sending data to server", 1);

    if (recv(sockfd, server_answer, 10, 0) <= 0)
        exit_prog("Error receiving data from server", 1);

    if (strcmp(server_answer, "bad") == 0)
    {
        puts("file already exists");
        return;
    }
    send_file(username, filename, sockfd);
}

void handle_file_download(int sockfd, char * username)
{
    char filename[SIZE] = {0};
    char server_answer[10] = {0};
    puts("please enter the name of the file you want to download");
    gets(filename);

    if (send(sockfd, filename, strlen(username), 0) == -1)
        exit_prog("Error sending data to server", 1);

    if (recv(sockfd, server_answer, 10, 0) <= 0)
        exit_prog("Error receiving data from server", 1);
        
    if (strcmp(server_answer, "bad") == 0)
    {
        puts("file does not exists");
        return;
    }
    write_file(username, filename, sockfd);
}

void client_menu()
{
    int answer = 0;
    int sockfd;
    struct sockaddr_in server_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    char username[SIZE] = {0};

    if(sockfd < 0) 
        exit_prog("Error in socket", 1);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = PORT;
    server_addr.sin_addr.s_addr = inet_addr(IP);

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
        exit_prog("Error in connect", 1);

    puts("please enter username : ");
    gets(username);
    if (send(sockfd, username, strlen(username), 0) == -1)
        exit_prog("Error sending data to server", 1);

    while (answer != 3)
    {
        print_menu();
        scanf("%d", &answer);
        switch (answer)
        {
            case 1:
                handle_file_upload(sockfd, username);
                break;
            case 2:
                handle_file_download(sockfd, username);
                break;
            case 3:
                puts("bye bye!");
                break;
            default:
                puts("wrong input. please try again");
        }
    }
    close(sockfd);
}

int main()
{
    client_menu();
    return 0;
}