#include "client.h"

int create_socket(int *sock, struct sockaddr_in *server) {

  *sock = socket(AF_INET, SOCK_STREAM, 0);
  if (*sock == -1) {
    printf("Could not create socket\n");
    return -1;
  }
  printf("Socket created\n");

  (*server).sin_addr.s_addr = inet_addr("127.0.0.1");
  (*server).sin_family = AF_INET;
  (*server).sin_port = htons(8080);

  return 1;
}

int connect_to_server(int *sock, struct sockaddr_in *server) {

  char message[1000];
  char server_reply[2000];

  if (connect(*sock, (struct sockaddr *)server, sizeof((*server))) < 0) {
    perror("Connection failed\n");
    return -1;
  }
  printf("Connected\n");

  if (send_message(sock, message, server_reply) != 1) {
    perror("Error in send_message()\n");
    return -1;
  }  

  printf("Closing connection\n");
  close(*sock);

  return 1;
}

int send_message(int *sock, char message[1000], char server_reply[2000]) {
  
  while (1) {
    printf("Enter message: ");
    scanf("%s", message);

    if (send(*sock, message, strlen(message), 0) < 0) {
      perror("Send failed\n");
      return -1;
    }

    if (recv(*sock, server_reply, 2000, 0) < 0) {
      perror("Receiving failed\n");
      return -1;
    }
    puts("Server reply: ");
    puts(server_reply);
  }
    
    return 1;
}

int main(void) {

  int sock;
  struct sockaddr_in server;

  if (create_socket(&sock, &server) != 1) {
    perror("Error creating socket\n");
    return -1;
  }

  if (connect_to_server(&sock, &server) != 1) {
    perror("Error connecting to server\n");
    return -1;
  }

  return 0;
}
