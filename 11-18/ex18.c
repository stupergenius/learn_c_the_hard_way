#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

void die(const char *message) {
  if (errno) {
    perror(message);
  } else {
    printf("ERROR: %s\n", message);
  }

  exit(1);
}

// new type to a function pointer
typedef int (*compare_cb)(int a, int b);
typedef int *(*sort)(int *numbers, int count, compare_cb cmp);

// classic bubble sort
int *bubble_sort(int *numbers, int count, compare_cb cmp) {
  int temp = 0;
  int i = 0;
  int j = 0;
  int *target = malloc(count * sizeof(int));

  if (!target) die("Memory error");

  memcpy(target, numbers, count * sizeof(int));

  for(i = 0; i < count; i++) {
    for(j = 0; j < count - 1; j++) {
      if (cmp(target[j], target[j+1]) > 0) {
        temp = target[j+1];
        target[j+1] = target[j];
        target[j] = temp;
      }
    }
  }

  return target;
}

int sorted_order(int a, int b) {
  return a - b;
}

int reverse_order(int a, int b) {
  return b - a;
}

int strange_order(int a, int b) {
  if (a == 0 || b == 0) {
    return 0;
  } else {
    return a % b;
  }
}

// test the thing
void test_sorting(int *numbers, int count, sort sorter, compare_cb cmp) {
  int i = 0;
  int *sorted = sorter(numbers, count, cmp);

  if (!sorted) die("Failed to sort as request.");

  for(i = 0; i < count; i++) {
    printf("%d ", sorted[i]);
  }
  puts("");

  free(sorted);
}

int main(int argc, char *argv[]) {
  if (argc < 2) die("USAGE: ex18 4 3 1 5 6");

  int count = argc - 1;
  int i =0;
  char **inputs = argv + 1;

  int *numbers = malloc(count * sizeof(int));
  if (!numbers) die("Memory error");

  for(i = 0; i < count; i++) {
    numbers[i] = atoi(inputs[i]);
  }

  test_sorting(numbers, count, bubble_sort, sorted_order);
  test_sorting(numbers, count, bubble_sort, reverse_order);
  test_sorting(numbers, count, bubble_sort, strange_order);

  free(numbers);

  return 0;
}
