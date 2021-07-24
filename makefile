server:
	gcc -o debug/server server.c utils.c
client:
	gcc -o debug/client client.c utils.c
main:
	gcc -o debug/main main.c