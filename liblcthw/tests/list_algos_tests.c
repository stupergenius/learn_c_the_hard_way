#include "minunit.h"
#include <lcthw/list_algos.h>
#include <assert.h>
#include <string.h>

char *values[] = { "XXXX", "1234", "abcd", "xjvef", "NDSS" };

#define NUM_VAUES 5

List *create_words() {
  int i = 0;
  List *words = List_create();

  for(i = 0; i < NUM_VAUES; i++) {
    char *v = calloc(1, sizeof(values[i]));
    List_push(words, strcpy(v, values[i]));
  }

  return words;
}

int is_sorted(List *words) {
  LIST_FOREACH(words, first, next, cur) {
    if (cur->next && strcmp(cur->value, cur->next->value) > 0) {
      debug("%s %s", (char *)cur->value, (char *)cur->next->value);
      return 0;
    }
  }

  return 1;
}

char *test_bubble_sort() {
  List *words = create_words();
  List_compare compare = (List_compare) strcmp;

  // should work on a list that's not pre-sorted
  int rc = List_bubble_sort(words, compare);
  // debug("rc = %d", rc);
  mu_assert(rc == 0, "bubble sort failed");
  mu_assert(is_sorted(words), "words arent sorted after bubble sort");

  // should work on a pre-sorted list
  rc = List_bubble_sort(words, compare);
  mu_assert(rc == 0, "bubble sort pre-sorted failed");
  mu_assert(is_sorted(words), "words arent sorted after pre-sorting bubble sort");

  // List_destroy(words);

  // should work on an empty list
  List *empty = List_create();
  rc = List_bubble_sort(empty, compare);
  mu_assert(rc == 0, "bubble sort failed");
  mu_assert(is_sorted(empty), "words arent sorted after bubble sort");

  List_destroy(empty);

  return NULL;
}

char *test_merge_sort() {
  List *words = create_words();
  List_compare compare = (List_compare) strcmp;

  // should work on a list that's not pre-sorted
  List *res1 = List_merge_sort(words, compare);
  mu_assert(is_sorted(res1), "words arent sorted after merge sort");

  // should work on a pre-sorted list
  List *res2 = List_merge_sort(res1, compare);
  mu_assert(is_sorted(res2), "words arent sorted after pre-sorted merge sort");

  // List_destroy(res1);
  // List_destroy(res2);
  // List_destroy(words);

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_bubble_sort);
  mu_run_test(test_merge_sort);

  return NULL;
}

RUN_TESTS(all_tests);