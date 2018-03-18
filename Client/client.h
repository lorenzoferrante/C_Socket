#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "utils.c"

int create_socket(char *server_addr, int *sock, struct sockaddr_in *server, int port);

int connect_to_server(int *sock, struct sockaddr_in *server);

int send_message(const int *sock, struct PAYLOAD *data, char server_reply[MAXBUFLEN]);

struct PAYLOAD *read_message(const int *sock);

#endif
