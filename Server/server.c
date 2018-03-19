#include "server.h"
#include <sys/ioctl.h>
#include <net/if.h>

int create_socket_desc(int *socket_desc, struct sockaddr_in *server, int port) {

    *socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (*socket_desc == -1) {
        printf("Could not create socket\n");
        return -1;
    }
    printf("[Info]\tSocket created\n");

    (*server).sin_family = AF_INET;
    (*server).sin_addr.s_addr = INADDR_ANY;
    (*server).sin_port = htons(port);

    return 1;
}

int bind_socket(const int *socket_desc, struct sockaddr_in *server) {

    if (bind(*socket_desc, (struct sockaddr *)server, sizeof((*server))) < 0) {
        perror("Bind failed\n");
        return -1;
    }
    printf("[Info]\tBind done\n");

    listen(*socket_desc, 3);

    return 1;
}

int accept_connection(const int *socket_desc, int *client_sock, int *c, struct sockaddr_in *client, int port) {

    struct PAYLOAD *data = malloc(sizeof(struct PAYLOAD));

    printf("[Info]\tListening at %s:%d\n", get_address(), port);
    printf("[Info]\tWaiting for incoming connections\n");

    *c = sizeof(struct sockaddr_in);

    *client_sock = accept(*socket_desc, (struct sockaddr *)client, (socklen_t *)c);
    if (*client_sock < 0) {
        perror("Accept failed\n");
        return -1;
    }
    printf(ANSI_COLOR_MAGENTA "\n[Info]\tConnection accepted\n" ANSI_COLOR_MAGENTA);
    printf(ANSI_COLOR_RESET "\n" ANSI_COLOR_RESET);

    if (read_from_client(client_sock, data) != 1)
        perror("Error reading from client\n");

    return 1;
}

int read_from_client(const int *client_sock, struct PAYLOAD *data) {
    ssize_t read_size;

    while ((read_size = recv(*client_sock, data, sizeof(struct PAYLOAD), 0)) > 0) {
        printf("[Info]\tClient message: %s\n", data->message);

        if (send_to_client(client_sock) != 1) {
            perror("Error in send_to_client()");
            return -1;
        }
    }

    if (read_size == 0) {
        printf(ANSI_COLOR_RED "[Info]\tClient disconnected\n" ANSI_COLOR_RED);
        printf(ANSI_COLOR_RESET "\n" ANSI_COLOR_RESET);
        fflush(stdout);
    } else if (read_size == -1)
        perror("Receiving failed\n");


    return 1;

}

int send_to_client(const int *client_sock) {

    struct PAYLOAD *response = malloc(sizeof(struct PAYLOAD));

    response->file_type = -1;
    strcpy(response->message, "OK");
    if (write(*client_sock, response, sizeof(struct PAYLOAD)) != sizeof(struct PAYLOAD)) {
        printf("Error sending response to client\n");
        return -1;
    }

    return 1;
}

char *get_address() {

    int fd;
    struct ifreq ifr;

    char iface[] = "en0";

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    //Type of address to retrieve - IPv4 IP address
    ifr.ifr_addr.sa_family = AF_INET;

    //Copy the interface name in the ifreq structure
    strncpy(ifr.ifr_name , iface , IFNAMSIZ-1);

    ioctl(fd, SIOCGIFADDR, &ifr);

    close(fd);

    //display result
    char *ipAddr = inet_ntoa(((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr);
    return ipAddr;
}

int main(int argc, char *argv[]) {

    int socket_desc, client_sock, c, d, port = 8000;
    int pFlag = 0;
    struct sockaddr_in server, client;

    while ((d = getopt(argc, argv, "p:")) != -1) {
        switch (d) {
            case 'p':
                pFlag = 1;
                port = atoi(optarg);
                break;
            default:
                exit(-1);
        }
    }

    char *public_ip = get_public_ip();
    printf(ANSI_COLOR_GREEN "[Public IP]\t%s\n" ANSI_COLOR_GREEN, public_ip);

    char *ipAddr = get_address();
    printf(ANSI_COLOR_CYAN "[Local IP]\t%s\n" ANSI_COLOR_CYAN, ipAddr);
    printf(ANSI_COLOR_RESET "\n" ANSI_COLOR_RESET);

    if (pFlag == 0) {
        printf("[Usage] ./server -p [port]\n");
        exit(-1);
    }

    if (create_socket_desc(&socket_desc, &server, port) != 1)
        perror("Error creating socket\n");

    if (bind_socket(&socket_desc, &server) != 1)
        perror("Error binding socket\n");

    if (accept_connection(&socket_desc, &client_sock, &c, &client, port) != 1)
        perror("Error accepting connection\n");

    return 0;

}
