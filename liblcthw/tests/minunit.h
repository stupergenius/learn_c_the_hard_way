#undef NDEBUG
#ifndef _minunit_h
#define _minunit_h

#include <stdio.h>
#include <stdlib.h>
#include "../src/lcthw/dbg.h"

#define mu_suit_start() char *message = NULL

#define mu_assert(test, message) if (!test) { log_err(message); return message; }
#define mu_run_test(test) debug("\n------%s", " " #test); message = test(); tests_run++; if (message) return message;

#define RUN_TESTS(name) int main(int argc, char *argv[]) { \
  printf("------\nRUNNING: %s\n", argv[0]); \
  char *result = name(); \
  if (result != 0) { \
    printf("FAILED: %s\n", result); \
  } \
  exit(result != 0); \
}

int tests_run = 0;

#endif