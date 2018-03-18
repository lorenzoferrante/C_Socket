#include "server.h"

int create_socket_desc(int *socket_desc, struct sockaddr_in *server, int port) {

  *socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (*socket_desc == -1) {
    printf("Could not create socket\n");
    return -1;
  } 
  printf("Socket created\n");

  (*server).sin_family = AF_INET;
  (*server).sin_addr.s_addr = INADDR_ANY;
  (*server).sin_port = htons(port);

  return 1;
}

int bind_socket(int *socket_desc, struct sockaddr_in *server) {

  if (bind(*socket_desc, (struct sockaddr *)server, sizeof((*server))) < 0) {
    perror("Bind failed\n");
    return -1;
  }
  printf("Bind done\n");

  listen(*socket_desc, 3);
  printf("Listening...\n");

  return 1;
}

int accept_connection(int *socket_desc, int *client_sock, int *c, struct sockaddr_in *client) {

  char client_message[2000];

  printf("Waiting for incoming connections...\n");

  *c = sizeof(struct sockaddr_in);

  *client_sock = accept(*socket_desc, (struct sockaddr *)client, (socklen_t *)c);
  if (*client_sock < 0) {
    perror("Accept failed\n");
    return -1;
  }
  printf("Connection accepted\n");

  if (read_from_client(client_sock, client_message) != 1) 
    perror("Error reading from client\n");

  return 1;
}

int read_from_client(int *client_sock, char client_message[2000]) {
  int read_size;

  while ((read_size = recv(*client_sock, client_message, 2000, 0)) > 0) {
    puts("Client message: ");
    puts(client_message);
    write(*client_sock, client_message, strlen(client_message));  
  }

  if (read_size == 0) {
    printf("Client disconnected\n");
    fflush(stdout);
  } else if (read_size == -1) 
    perror("Receiving failed\n");

  return 1;

}

int main() {

  int socket_desc, client_sock, c;
  struct sockaddr_in server, client;
  
  if (create_socket_desc(&socket_desc, &server, 8080) != 1) 
    perror("Error creating socket\n");

  if (bind_socket(&socket_desc, &server) != 1)
    perror("Error binding socket\n");
  
  if (accept_connection(&socket_desc, &client_sock, &c, &client) != 1)
    perror("Error accepting connection\n");

  return 0;

}
