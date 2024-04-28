#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "Server.h"
#include "arpa/inet.h"

#define CLOSING_CHAR '@' // Define the closing character

void launch(struct Server* server);

#endif