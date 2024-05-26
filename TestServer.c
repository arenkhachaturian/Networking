#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "Server.h"
#include "arpa/inet.h"
#include "Launcher.h"

int main(void) {
  char* port = getenv("PORT_NUM");
  if (NULL == port) {
    return EXIT_FAILURE;
  }
  int port_num = atoi(port);
  printf("port number is: %d\n", port_num);

  struct Server server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, port_num, 10, launch);
  server.launch(&server);

  return 0;
}