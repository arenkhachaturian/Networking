#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "Server.h"
#include "DBReader.h"
#include "Utils.h"


#define MAX_CLIENTS 10
#define BUFFER_SIZE 3000
#define USER_NAME_SZ 32
#define PASSWORD_SZ 32

struct Credentials {
  char username[USER_NAME_SZ];
  char passowrd[PASSWORD_SZ];
};

int auth_user(struct Credentials* creds) {
    char buffer[1000];
    if  (creds == NULL)
        return -1;
    
    printf("Hello, guys!\n");

    FILE* userDBfile = fopen("users.csv", "r");
    if (userDBfile == NULL) {
        exit(-1);
    }
    printf("We opened file\n");

    fgets(buffer, sizeof(buffer), userDBfile);
    printf("I read some data\n");
    printf("%s\n", buffer);
    return 1;
}

int get_credentials(struct Credentials* creds, int client_socket) {
    char username[USER_NAME_SZ];
    char password[PASSWORD_SZ];
    const char promptUserName[] = "Please enter your username: ";
    int bytes_written = write(client_socket, promptUserName, sizeof(promptUserName));
    
    int bytes_read = read(client_socket, username, USER_NAME_SZ);
    if (bytes_read > 0) {
      username[bytes_read-2] = '\0';
      ntwrk_println("bytes read in username: %d", bytes_read);
      strcpy(creds->username, username);
    }

    const char promptPassword[] = "Please enter your username: ";
    bytes_written = write(client_socket, promptPassword, sizeof(promptPassword));
    bytes_read = read(client_socket, password, PASSWORD_SZ);
    if (bytes_read > 0) {
      password[bytes_read-2] = '\0';
      ntwrk_println("bytes read in password: %d", bytes_read);
      strcpy(creds->passowrd, password);
    }
}

void* handle_connection(void *arg) {
    int client_socket = *((int *)arg);
    free(arg);

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;

    ntwrk_println("Connection accepted. Starting session...");
    struct Credentials *ptr_credentials = (struct Credentials *)malloc(sizeof(struct Credentials));
    get_credentials(ptr_credentials, client_socket);
    int verification_result = verify_user(ptr_credentials->username, ptr_credentials->passowrd);
    if (verification_result == 0) {
      ntwrk_println("Hi dear %s!", ptr_credentials->username);
    } else if (verification_result > 0) {
      ntwrk_println("You are not welcomed anymore %s!", ptr_credentials->username);
      close(client_socket);
      pthread_exit(NULL);
    }
    while ((bytes_read = read(client_socket, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(client_socket, buffer, bytes_read);
        if (bytes_written == -1) {
            perror("Error writing to socket");
            break;
        }
    }

    if (bytes_read == 0) {
        ntwrk_println("Client disconnected.");
    } else if (bytes_read == -1) {
        perror("Error reading from socket");
    }

    close(client_socket);
    pthread_exit(NULL);
}

void launch(struct Server *server) {
    ntwrk_println("Waiting for connections...");

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
