#include "dbg.h"
#include <stdlib.h>
#include <stdio.h>

void test_debug() {
  // notice we dont need a newline
  debug("I have brown hair...");

  // passing in arguments
  debug("I am %d years old", 36);
}

void test_log_err() {
  log_err("I believe everything is broken");
  log_err("There are %d problems in %s", 0, "space");
}

void test_log_warn() {
  log_warn("You can safely ignore this.");
  log_warn("Maybe consider looking at %s", "/etc/passwd");
}

void test_log_info() {
  log_info("Well i did something mundane");
  log_info("It happend %.2f times today", 1.3f);
}

int test_check(char *filename) {
  FILE *input = NULL;
  char *block = NULL;

  block = malloc(100);
  check_mem(block);

  input = fopen(filename, "r");
  check(input, "Failed to open file: %s", filename);

  free(block);
  fclose(input);

  return 0;

  error:
    if (block) free(block);
    if (input) fclose(input);
    return 1;
}

int test_sentinel(int code) {
  char *temp = malloc(100);
  check_mem(temp);

  switch(code) {
    case 1:
      log_info("It worked!");
      break;

    default:
      sentinel("This shouldn't run");
  }

  return 0;

  error:
    if (temp) free(temp);
    return -1;
}

int test_check_mem() {
  char *test = NULL;
  check_mem(test);

  free(test);
  return 0;

  error:
    return -1;
}

int test_check_debug() {
  int i = 0;
  check_debug(i != 0, "Oops, I was 0");

  return 0;

  error:
    return -1;
}

int main(int argc, char *argv[]) {
  check(argc == 2, "Need an argument.");

  test_debug();
  test_log_err();
  test_log_warn();
  test_log_info();

  check(test_check("ex20.c") == 0, "failed with %s", "ex20.c");
  check(test_check(argv[1]) == 0, "failed with argv");

  check(test_sentinel(1) == 0, "test_sentinel failed");
  // check(test_sentinel(2) == 0, "test_sentinel failed");

  check(test_check_mem() == 0, "test_check_mem failed");
  check(test_check_debug() == 0, "test_check_debug failed");

  return 0;

  error:
    return 1;
}