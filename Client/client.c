#include <stdlib.h>
#include <unistd.h>
#include "client.h"

int create_socket(char *server_addr, int *sock, struct sockaddr_in *server, int port) {

    *sock = socket(AF_INET, SOCK_STREAM, 0);
    if (*sock == -1) {
        printf("Could not create socket\n");
        return -1;
    }
    printf("Socket created\n");

    (*server).sin_addr.s_addr = inet_addr(server_addr);
    (*server).sin_family = AF_INET;
    (*server).sin_port = htons(port);

    return 1;
}

int connect_to_server(int *sock, struct sockaddr_in *server) {

    struct PAYLOAD *data = malloc(sizeof(struct PAYLOAD));
    char server_reply[MAXBUFLEN];

    if (connect(*sock, (struct sockaddr *)server, sizeof((*server))) < 0) {
        perror("Connection failed\n");
        return -1;
    }
    printf("Connected\n");

    if (send_message(sock, data, server_reply) != 1) {
        perror("Error in send_message()\n");
        return -1;
    }

    printf("Closing connection\n");
    close(*sock);

    return 1;
}

int send_message(const int *sock, struct PAYLOAD *data, char server_reply[MAXBUFLEN]) {

    int file_type;
    char path[1000];
    char message[MAXBUFLEN];
    char ext[4];

    while (1) {
        printf("Enter file path: ");
        scanf("%s", path);

        strcpy(ext, get_file_ext(path));
        file_type = get_file_type(ext);

        if (read_file(path, message) <= 0) {
            perror("Error reading file\n");
            return -1;
        }

        // Populate the PAYLOAD
        data->file_type = file_type;
        strcpy(data->message, message);


        if (send(*sock, data, sizeof(struct PAYLOAD), 0) < 0) {
            perror("Send failed\n");
            return -1;
        }

        if (read_message(sock) == NULL) {
            perror("Error in read_message()\n");
            return -1;
        }

    }

    return 1;
}

struct PAYLOAD *read_message(const int *sock) {

    struct PAYLOAD *reply = malloc(sizeof(struct PAYLOAD));

    if (recv(*sock, reply, sizeof(struct PAYLOAD), 0) < 0) {
        perror("Receiving failed\n");
        return NULL;
    }
    printf("Server reply: %s\n", reply->message);

    return reply;
}

int main(int argc, char *argv[]) {

    int sock, port = 8000, d;
    int pFlag = 0, aFlag = 0;
    char *ipAddr = NULL;
    struct sockaddr_in server;

    while ((d = getopt(argc, argv, "a:p:")) != -1) {
        switch (d) {
            case 'a':
                aFlag = 1;
                ipAddr = optarg;
                break;
            case 'p':
                pFlag = 1;
                port = atoi(optarg);
                break;
            default:
                exit(-1);
        }
    }

    if (pFlag == 0 || aFlag == 0) {
        printf("[Usage] ./server -a [address] -p [port]\n");
        exit(-1);
    }

    printf("Trying to connect to: %s:%d\n", ipAddr, port);

    if (create_socket(ipAddr, &sock, &server, port) != 1) {
        perror("Error creating socket\n");
        return -1;
    }

    if (connect_to_server(&sock, &server) != 1) {
        perror("Error connecting to server\n");
        return -1;
    }

    return 0;
}
