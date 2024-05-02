#include <string.h>
#include <stdio.h>
#include "DBReader.h"

#define USERS_FILE "../data/users.txt"

int verify_user(char* username, char* passowrd) {
    FILE* users_file;
    char  buffer[100];

    users_file = fopen(USERS_FILE, "r");

    if (users_file == NULL) {
      printf("Error opening file %s\n", USERS_FILE);
      return -1;
    }

    while (fgets(buffer, sizeof(buffer), users_file) != NULL) {
      char* tmp_username = strtok(buffer, ",");
      if (strncmp(tmp_username, username, sizeof(username)) == 0) {
        printf("User %s is found\n", username);
        char* tmp_password = strtok(NULL, ",");
          if (strncmp(tmp_password, passowrd, sizeof(passowrd)) == 0) {
            printf("User %s entered with password %s \n", username, passowrd);
            return 0;
          }
      }
    }

    printf("User %s not found\n", username);
    return 1;
}