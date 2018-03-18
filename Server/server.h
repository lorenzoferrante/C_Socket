#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "utils.c"

int create_socket_desc(int *socket_desc, struct sockaddr_in *server, int port);

int bind_socket(const int *socket_desc, struct sockaddr_in *server);

int accept_connection(const int *socket_desc, int *client_sock, int *c, struct sockaddr_in *client);

int read_from_client(const int *client_sock, struct PAYLOAD *data);

int send_to_client(const int *client_sock);

#endif
