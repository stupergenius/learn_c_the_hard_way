#include <stdio.h>

int main(int argc, char *argv[]) {
  int i = 0;

  if (argc == 1) {
    puts("You have no arguments.");
  } else if (argc > 1 && argc < 5) {
    puts("Here's your arguments:");

    for (i = 1; i < argc; i++) {
      printf("%s ", argv[i]);
    }
    puts("");
  } else {
    puts("You have too many arguments.");
  }

  return 0;
}