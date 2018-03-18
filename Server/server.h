#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int create_socket_desc(int *socket_desc, struct sockaddr_in *server, int port);

int bind_socket(int *socket_desc, struct sockaddr_in *server);

int accept_connection(int *socket_desc, int *client_sock, int *c, struct sockaddr_in *client);

int read_from_client(int *client_sock, char client_message[2000]); 

#endif
