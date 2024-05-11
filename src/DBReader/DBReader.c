#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "DBReader.h"

int verify_user(char* username, char* passowrd) {
  char* users_file_name = getenv("USER_DATA_FILE"); // always include stdlib.h
  if (NULL == users_file_name) {
    ntwrk_println("Data path is not set");
    return -1;
  }

  FILE* users_file;
  char  buffer[100];

  users_file = fopen(users_file_name, "r");

  if (users_file == NULL) {
    ntwrk_println("Error opening file %s\n", users_file_name);
    return -1;
  }

  while (fgets(buffer, sizeof(buffer), users_file) != NULL) {
    char* tmp_username = strtok(buffer, ",");
    if (strncmp(tmp_username, username, sizeof(username)) == 0) {
      ntwrk_println("User %s is found\n", username);
      char* tmp_password = strtok(NULL, ",");
      if (strncmp(tmp_password, passowrd, sizeof(passowrd)) == 0) {
        ntwrk_println("User %s entered with password %s \n", username, passowrd);
        return 0;
      }
    }
  }

  ntwrk_println("User %s not found\n", username);
  return 1;
}