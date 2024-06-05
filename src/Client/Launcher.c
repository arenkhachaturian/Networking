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


enum MenuOption
{
  MENU,
  OPTION_1, // temporary placeholder. real functionality should be implemented later
  OPTION_2,
  EXIT
};

int handle_menu(enum MenuOption* choice) {
  switch (*choice)
  {
  case 1:
    *choice = OPTION_1;
    break;
  case 2:
    *choice = OPTION_2;
    break;
  case 3:
    *choice = EXIT;
    break;
  default:
    *choice = MENU;
    return -1;
  }
  return 0;
}


int readChoice(int client_socket, enum MenuOption* state) {
  int intputBufferSize = 32;
  char inputBuffer[intputBufferSize];
  int bytes_read = read(client_socket, inputBuffer, intputBufferSize);
 
  if (bytes_read > 0) 
  {
    inputBuffer[bytes_read - 1] = '\0';
    *state = (enum MenuOption)atoi(inputBuffer);
    return 0;
  }

  return -1;
}


int handle_state(int client_socket)
{
  int bytes_read = 0;
  int bytes_written = 0;
  
  enum MenuOption state = MENU;
  while (state != EXIT)
  {
    switch (state)
    {
    case MENU:
      bytes_written = ntwrk_write(client_socket, "MENU:\n1. Option 1\n2. Option 2\n3. Exit\nEnter your choice: ");
      readChoice(client_socket, &state);
      if (handle_menu(&state) < 0)
      {
        int bytes_written = ntwrk_write(client_socket, "Invalid choice. Please try again.\n");
      }   
      break;
    case OPTION_1:
      bytes_written = ntwrk_write(client_socket, "You chose Option 1.\n");
      state = MENU;
      break;
    case OPTION_2:
      bytes_written = ntwrk_write(client_socket, "You chose Option 2.\n");
      state = MENU;
      break;
    case EXIT:
      bytes_written = ntwrk_write(client_socket, "Exiting...\n");
      break;
    }
  }
}

int get_credentials(struct Credentials* creds, int client_socket) {
  char username[USER_NAME_SZ];
  char password[PASSWORD_SZ];
  const char promptUserName[] = "Please enter your username: ";
  int bytes_written = write(client_socket, promptUserName, sizeof(promptUserName));

  int bytes_read = read(client_socket, username, USER_NAME_SZ);
  if (bytes_read > 0) {
    username[bytes_read - 2] = '\0';
    strcpy(creds->username, username);
  }

  const char promptPassword[] = "Please enter your password: ";
  bytes_written = write(client_socket, promptPassword, sizeof(promptPassword));
  bytes_read = read(client_socket, password, PASSWORD_SZ);
  if (bytes_read > 0) {
    password[bytes_read - 2] = '\0';
    strcpy(creds->passowrd, password);
  }
}

void* handle_connection(void* arg) {
  int client_socket = *((int*)arg);
  free(arg);

  char buffer[BUFFER_SIZE];
  ssize_t bytes_read, bytes_written;

  ntwrk_println("Connection accepted. Starting session...");
  struct Credentials* ptr_credentials = (struct Credentials*)malloc(sizeof(struct Credentials));
  get_credentials(ptr_credentials, client_socket);
  int verification_result = verify_user(ptr_credentials->username, ptr_credentials->passowrd);
  if (verification_result == 0) {
    ntwrk_println("Hi dear %s!", ptr_credentials->username);
  }
  else if (verification_result > 0) {
    ntwrk_println("You are not welcomed anymore %s!", ptr_credentials->username);
    close(client_socket);
    pthread_exit(NULL);
  } else {
    ntwrk_println("Error while reading dbg");
    close(client_socket);
    pthread_exit(NULL);
  }
  handle_state(client_socket);

  close(client_socket);
  pthread_exit(NULL);
}

void launch(struct Server* server) {
  ntwrk_println("Waiting for connections...");

  while (1) {
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_socket = accept(server->socket, (struct sockaddr*)&client_addr, &client_addr_len);
    if (client_socket < 0) {
      perror("Error accepting connection\n");
      continue;
    }

    pthread_t tid;
    int* client_socket_ptr = (int*)malloc(sizeof(int));
    if (client_socket_ptr == NULL) {
      perror("Error allocating memory\n");
      close(client_socket);
      continue;
    }
    *client_socket_ptr = client_socket;

    if (pthread_create(&tid, NULL, handle_connection, (void*)client_socket_ptr) != 0) {
      perror("Error creating thread\n");
      close(client_socket);
      free(client_socket_ptr);
      continue;
    }

    // Detach the thread
    pthread_detach(tid);
  }
}
