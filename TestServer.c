#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "Server.h"
#include "arpa/inet.h"

#define CLOSING_CHAR '@' // Define the closing character

void launch(struct Server* server) {
  printf("Server listening on %s:%d\n", inet_ntoa(server->address.sin_addr), ntohs(server->address.sin_port));
  printf("===== Waiting to connect ====\n");
  while(1) {
    int buffer_sz = 3000;
    char buffer[buffer_sz];
    struct sockaddr* addr = (struct sockaddr*) &(server->address);
    socklen_t addr_length = sizeof(server->address);
    int new_socket = accept(server->socket, addr, &addr_length);
    if (new_socket < 0) {
      perror("Error accepting connection");
      return;
    }
    printf("Client connected\n");

    while (1) {
      ssize_t bytes_read = read(new_socket, buffer, buffer_sz);
      if (bytes_read <= 0) {
        if (bytes_read == 0) {
          printf("Client disconnected\n");
        } else {
          perror("Error while reading from socket");
        }
        close(new_socket);
        break;
      }

      // Check if the received data contains the closing character
      int i;
      for (i = 0; i < bytes_read; i++) {
        if (buffer[i] == CLOSING_CHAR) {
          printf("Client sent closing character '@'. Disconnecting.\n");
          close(new_socket);
        } else if (buffer[i] == '\0') { // Handle escape character
          printf("Escape character detected. Ignoring.\n");
        } else {
          ssize_t bytes_written = write(new_socket, &buffer[i], sizeof(buffer[i]));
        }
      }
    }
  }
}

int main(void) {
  struct Server server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 8080, 10, launch);
  server.launch(&server);

  return 0;
}