cmake_minimum_required(VERSION 3.17)
project(FileStorageServerC C)

set(CMAKE_C_STANDARD 99)

add_executable(FileStorageServerC main.c utils.c utils.h server.c client.c)