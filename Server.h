#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>

struct Server
{
  int domain;
  int service;
  int protocol;
  unsigned interface;
  int port;
  int backlog;

  struct sockaddr_in address;

  int socket;

  void (*launch)(struct Server*);
};

struct Server server_constructor(int domain, int service, int protocol, 
                                unsigned interface, int port,
                                int backlog, void (*launch)(struct Server*));

#endif /* Server.h */