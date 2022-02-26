#undef NDEBUG
#ifndef _minunit_h
#define _minunit_h

#include <stdio.h>
#include <stdlib.h>
#include "../src/lcthw/dbg.h"

#define NON_NULL(x) ((x) ? (x) : "")

#define mu_suite_start() char *message = NULL

#define mu_assert(test, message) if (!(test)) { log_err(message); return message; }
#define mu_run_test(test) debug("\n------%s", " " #test); message = test(); tests_run++; if (message) return message;

#define RUN_TESTS(name) int main(int argc __attribute__((unused)), char *argv[]) { \
  printf("------\nRUNNING: %s\n", argv[0]); \
  char *result = name(); \
  if (result != 0) { \
    printf("FAILED: %s\n", result); \
  } \
  printf("Tests run: %d\n", tests_run); \
  exit(result != 0); \
}

int tests_run = 0;

#endif