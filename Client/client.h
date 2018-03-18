#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int create_socket(int *sock, struct sockaddr_in *server);

int connet_to_server(int *sock, struct sockaddr_in *server);

int send_message(int *sock, char message[1000], char server_reply[2000]);

#endif
