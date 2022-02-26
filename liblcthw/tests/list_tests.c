#include <assert.h>
#include "minunit.h"
#include <lcthw/list.h>

static List *list = NULL;
char *test1 = "test 1 data";
char *test2 = "test 2 data";
char *test3 = "test 3 data";

char *test_create() {
  list = List_create();
  mu_assert(list != NULL, "Failed to create list");
  return NULL;
}

char *test_destroy() {
  List_clear_destroy(list);
  return NULL;
}

char *test_push() {
  List_push(list, test1);
  mu_assert(List_last(list) == test1, "Wrong last value");

  List_push(list, test2);
  mu_assert(List_last(list) == test2, "Wrong last value");

  List_push(list, test3);
  mu_assert(List_last(list) == test3, "Wrong last value");

  mu_assert(List_count(list) == 3, "Wrong count value");
  return NULL;
}

char *test_pop() {
  char *popped = NULL;

  popped = List_pop(list);
  mu_assert(popped == test3, "wrong pop value");
  mu_assert(List_last(list) == test2, "wrong last value");
  mu_assert(List_count(list) == 2, "wrong count value");

  popped = List_pop(list);
  mu_assert(popped == test2, "wrong pop value");
  mu_assert(List_last(list) == test1, "wrong last value");
  mu_assert(List_count(list) == 1, "wrong count value");

  popped = List_pop(list);
  mu_assert(popped == test1, "wrong pop value");
  mu_assert(List_last(list) == NULL, "wrong last value");
  mu_assert(List_count(list) == 0, "wrong count value after popping");

  return NULL;
}

char *test_unshift() {
  List_unshift(list, test1);
  mu_assert(List_first(list) == test1, "wrong first value");
  mu_assert(List_count(list) == 1, "wrong count value");

  List_unshift(list, test2);
  mu_assert(List_first(list) == test2, "wrong first value");
  mu_assert(List_count(list) == 2, "wrong count value");

  List_unshift(list, test3);
  mu_assert(List_first(list) == test3, "wrong first value");
  mu_assert(List_count(list) == 3, "wrong count value");

  return NULL;
}

char *test_remove() {
  void *removed = NULL;

  // list state is (from test_unshift): test3, test2, test1

  // middle value first
  removed = List_remove(list, list->first->next);
  mu_assert(removed == test2, "wrong removed value");
  mu_assert(List_count(list) == 2, "wrong count value");
  mu_assert(List_last(list) == test1, "wrong last value");
  mu_assert(List_first(list) == test3, "wrong first value");
  mu_assert(list->first->next == list->last, "list pointers wrong");
  mu_assert(list->last->prev == list->first, "list pointers wrong");

  // then last
  removed = List_remove(list, list->last);
  mu_assert(removed == test1, "wrong removed value");
  mu_assert(List_last(list) == test3, "wrong last value");
  mu_assert(List_count(list) == 1, "wrong count value");

  // and first
  removed = List_remove(list, list->first);
  mu_assert(removed == test3, "wrong removed value");
  mu_assert(List_count(list) == 0, "wrong count value");

  return NULL;
}

char *test_shift() {
  List_unshift(list, test3);
  List_unshift(list, test2);
  List_unshift(list, test1);

  char *shifted = NULL;

  // list state is : test1, test2, test3

  shifted = List_shift(list);
  mu_assert(shifted == test1, "wrong shifted value");
  mu_assert(List_count(list) == 2, "wrong count value");
  mu_assert(List_last(list) == test3, "wrong last value");

  shifted = List_shift(list);
  mu_assert(shifted == test2, "wrong shifted value");
  mu_assert(List_count(list) == 1, "wrong count value");
  mu_assert(List_last(list) == test3, "wrong last value");

  shifted = List_shift(list);
  mu_assert(shifted == test3, "wrong shifted value");
  mu_assert(List_count(list) == 0, "wrong count value");
  mu_assert(List_last(list) == NULL, "wrong last value");

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_push);
  mu_run_test(test_pop);
  mu_run_test(test_unshift);
  mu_run_test(test_remove);
  mu_run_test(test_shift);
  mu_run_test(test_destroy);

  return NULL;
}

RUN_TESTS(all_tests);