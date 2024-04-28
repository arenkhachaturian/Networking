#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "Server.h"
#include "arpa/inet.h"
#include "Launcher.h"

int main(void) {
  struct Server server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 8080, 10, launch);
  server.launch(&server);

  return 0;
}