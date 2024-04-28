#include <unistd.h>
#include "Launcher.h"

#define BUFFER_SIZE 3000

void handle_connection(int client_socket) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;

    printf("Connection accepted. Reading data...\n");

    while ((bytes_read = read(client_socket, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(client_socket, buffer, bytes_read);
        if (bytes_written == -1) {
            perror("Error writing to socket");
            break;
        }
    }

    if (bytes_read == 0) {
        printf("Client disconnected.\n");
    } else if (bytes_read == -1) {
        perror("Error reading from socket");
    }

    close(client_socket);
}

void launch(struct Server *server) {
    printf("Waiting for connections...\n");

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_socket = accept(server->socket, (struct sockaddr *)&client_addr, &client_addr_len);

        if (client_socket < 0) {
            perror("Error accepting connection");
            continue;
        }

        handle_connection(client_socket);
    }
}
