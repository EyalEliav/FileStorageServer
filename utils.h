#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

void create_file_path(char * path, char * username, char * filename);

void send_file(char * username, char * filename, int sockfd);

void write_file(char * username, char * filename, int sockfd);

void error_exit(char * error, int exit_code);