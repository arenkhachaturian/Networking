#include <stdint.h> // For uint32_t
#include <limits.h> // for CHAR_BIT
#include <stdlib.h>
#include "PrimeNumbers.h"

int fillSieve(struct Sieve* sieve, unsigned num) {
  int size = num;
  sieve->arr = calloc(size, sizeof(char));
  for (int i = 2; i * i < size; ++i) {
    if (is_prime(sieve, i)) {
      for (int j = i * i; j <= size; j += i) {
        set_prime(sieve, j);
      }
    }
  }
}

void set_prime(struct Sieve* sieve, unsigned num) {
  int bytenum = num / CHAR_BIT;
  int bitnum = num % CHAR_BIT;
  sieve->arr[bytenum] = sieve->arr[bytenum] | (1 << bitnum);
}

int is_prime(struct Sieve* sieve, int num) {
  int bytenum = num / CHAR_BIT;
  int bitnum = num % CHAR_BIT;
  int x = (sieve->arr[bytenum] >> bitnum) & 1;
  return (x == 0) ? 1 : 0;
}
