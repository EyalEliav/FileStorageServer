server:
	gcc -o server_folder/server server.c utils.c
client:
	gcc -o client_folder/client client.c utils.c
main:
	gcc -o debug/main main.c
all:
	make client
	make server