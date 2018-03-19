#ifndef SERVER_H
#define SERVER_H

#include "utils.c"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

char *get_address();

int create_socket_desc(int *socket_desc, struct sockaddr_in *server, int port);

int bind_socket(const int *socket_desc, struct sockaddr_in *server);

int accept_connection(const int *socket_desc, int *client_sock, int *c, struct sockaddr_in *client, int port);

int read_from_client(const int *client_sock, struct PAYLOAD *data);

int send_to_client(const int *client_sock);

#endif
