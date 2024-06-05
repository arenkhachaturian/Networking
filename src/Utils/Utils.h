#ifndef UTILS_H
#define UTILS_H

void ntwrk_println(const char* format, ...);

int ntwrk_write(int client_socket, const char* str);

#endif