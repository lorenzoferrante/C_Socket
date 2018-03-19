#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define MAXBUFLEN 100000

struct PAYLOAD {
    int file_type;
    char message[MAXBUFLEN];
};

int get_file_type(char *ext) {
    if (strcmp(ext, "txt") == 0)
        return 0;
    if (strcmp(ext, "jpg") == 0)
        return 1;
    return -1;
}

char *get_file_ext(char *path) {

    size_t len = strlen(path);
    char *ext = (char *) malloc(sizeof(char) * 4);
    int i, j;

    j = 2;
    for (i = (int)len - 1; i >= 0; i--) {
        ext[j] = path[i];
        j--;
    }
    ext[3] = '\0';

    return ext;
}

size_t read_file(char *path, char *source) {
    char *cpStr = malloc(sizeof(char) * MAXBUFLEN);

    FILE *fd = fopen(path, "r");
    if (fd == 0) {
        printf("Error reading file from path: %s\n", path);
        return (size_t) -1;
    }

    size_t readLen = fread(cpStr, sizeof(char), MAXBUFLEN, fd);
    if (ferror(fd) != 0) {
        perror("Error reading file\n");
        return (size_t) -1;
    }

    cpStr[readLen++] = '\0';
    strcpy(source, cpStr);

    fclose(fd);

    return readLen;
}

void error(const char *msg) { perror(msg); exit(0); }

char *get_public_ip() {
    /* first what are we going to send and where are we going to send it? */
    int portno =        80;
    char *host =        "icanhazip.com";
    char *message_fmt = "GET / HTTP/1.1\r\nHost: icanhazip.com\r\n\r\n";

    struct hostent *server;
    struct sockaddr_in serv_addr;
    int sockfd, bytes, sent, received, total;
    char message[1024],response[4096];

    /* fill in the parameters */
    sprintf(message,message_fmt,NULL,NULL);
    //printf("Request:\n%s\n",message);

    /* create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");

    /* lookup the ip address */
    server = gethostbyname(host);
    if (server == NULL) error("ERROR, no such host");

    /* fill in the structure */
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);

    /* connect the socket */
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    /* send the request */
    total = strlen(message);
    sent = 0;
    do {
        bytes = write(sockfd,message+sent,total-sent);
        if (bytes < 0)
            error("ERROR writing message to socket");
        if (bytes == 0)
            break;
        sent+=bytes;
    } while (sent < total);

    /* receive the response */
    memset(response,0,sizeof(response));
    total = sizeof(response)-1;
    received = 0;
    do {
        bytes = read(sockfd,response+received,total-received);
        if (bytes < 0)
            error("ERROR reading response from socket");
        if (bytes == 0)
            break;
        received+=bytes;
    } while (received < total);

    if (received == total)
        error("ERROR storing complete response from socket");

    /* close the socket */
    close(sockfd);

    /* process response */
    //printf("Response:\n%s\n",response);

    /* get public ip address */
    char *ptr = strstr(response, "Content-Length: ");
    char content_len[1024];
    char public_ip[32];
    char *tmp;

    int x = 16, y = 0;
    while (ptr[x] != '\n') {
        content_len[y] = ptr[x];
        x++;
        y++;
    }
    content_len[y] = '\0';
    tmp = &response[strlen(response) - atoi(content_len)];
    strcpy(public_ip, tmp);

    printf("Public IP address: %s", public_ip);

    return public_ip;
}

