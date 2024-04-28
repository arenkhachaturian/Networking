#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "Server.h"

#define MAX_CLIENTS 10
#define BUFFER_SIZE 3000

void *handle_connection(void *arg) {
    int client_socket = *((int *)arg);
    free(arg);

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
    pthread_exit(NULL);
}

void launch(struct Server *server) {
    printf("Waiting for connections...\n");

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_socket = accept(server->socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            perror("Error accepting connection\n");
            continue;
        }

        pthread_t tid;
        int *client_socket_ptr = (int *)malloc(sizeof(int));
        if (client_socket_ptr == NULL) {
            perror("Error allocating memory\n");
            close(client_socket);
            continue;
        }
        *client_socket_ptr = client_socket;

        if (pthread_create(&tid, NULL, handle_connection, (void *)client_socket_ptr) != 0) {
            perror("Error creating thread\n");
            close(client_socket);
            free(client_socket_ptr);
            continue;
        }

        // Detach the thread
        pthread_detach(tid);
    }
}
