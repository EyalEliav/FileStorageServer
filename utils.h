char * create_file_path(char * username, char * filename);

void send_file(char * username, char * filename, int sockfd);

void write_file(char * username, char * filename, int sockfd);

void exit_prog(char * error, int exit_code);