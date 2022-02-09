#include <stdio.h>

int main(int argc, char *argv[]) {
  int i = argc - 1;
  while(i > 0) {
    int index = argc - i;
    printf("arg %d: %s\n", index, argv[index]);
    i--;
  }

  // make our own array
  char *states[] = {
    "California", "Oregon",
    "Washington", "Texas"
  };
  int num_states = 4;

  i = 0;
  while (i < num_states) {
    printf("state %d: %s\n", i, states[i]);
    i++;
  }

  return 0;
}
