#include <stdio.h>
#include <stdarg.h>
#include "Utils.h"

void ntwrk_println(const char* format, ...) {
  va_list  args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);

  putchar('\n');
}

