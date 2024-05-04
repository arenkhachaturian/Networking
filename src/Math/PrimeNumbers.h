#ifndef SIEVE_H
#define SIEVE_H

#include <stdint.h> // For uint32_t
#include <limits.h> // for CHAR_BIT

struct Sieve {
  char*    arr;
  unsigned size;
};


int fillSieve(struct Sieve* sieve, unsigned num);

void set_prime(struct Sieve* sieve, unsigned num);

int is_prime(struct Sieve* sieve, int num);

#endif /* SIEVE_H */
