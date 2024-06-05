#include <stdio.h>
#include <stdarg.h>
#include "Utils.h"
#include "arpa/inet.h"

void ntwrk_println(const char* format, ...) {
  va_list  args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);

  putchar('\n');
}

int ntwrk_write(int client_socket, const char* str) {
  int bytes_written = write(client_socket, str, strlen(str));
  if (bytes_written == -1)
  {
    perror("Error writing to socket");
    return bytes_written;
  }
  return bytes_written;
}
