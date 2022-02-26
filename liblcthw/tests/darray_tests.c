#include "minunit.h"
#include <lcthw/darray.h>

static DArray *array = NULL;
static int *val1 = NULL;
static int *val2 = NULL;

char *test_create() {
  array = DArray_create(sizeof(int), 100);
  mu_assert(array != NULL, "DArray_create failed");
  mu_assert(array->contents != NULL, "contents are wrong in DArray");
  mu_assert(array->end == 0, "end isnt in the right spot");
  mu_assert(array->element_size == sizeof(int), "element size is wrong");
  mu_assert(array->max == 100, "wrong max length of initial size");

  return NULL;
}

char *test_destroy() {
  DArray_destroy(array);

  return NULL;
}

char *test_new() {
  val1 = DArray_new(array);
  mu_assert(val1 != NULL, "failed to make a new element");

  val2 = DArray_new(array);
  mu_assert(val2 != NULL, "failed to make a new element");

  return NULL;
}

char *test_set() {
  DArray_set(array, 0, val1);
  DArray_set(array, 1, val2);

  return NULL;
}

char *test_get() {
  mu_assert(DArray_get(array, 0) == val1, "wrong first value");
  mu_assert(DArray_get(array, 1) == val2, "wrong second value");

  return NULL;
}

char *test_remove() {
  int *val_check = DArray_remove(array, 0);
  mu_assert(val_check != NULL, "should not get null");
  mu_assert(*val_check == *val1, "should get the first value");
  mu_assert(DArray_get(array, 0) == NULL, "should be gone");
  DArray_free(val_check);

  val_check = DArray_remove(array, 1);
  mu_assert(val_check != NULL, "should not get null");
  mu_assert(*val_check == *val2, "should get the second value");
  mu_assert(DArray_get(array, 1) == NULL, "should be gone");
  DArray_free(val_check);

  return NULL;
}

char *test_expand_contract() {
  int old_max = array->max;

  DArray_expand(array);
  mu_assert((unsigned int)array->max == old_max + array->expand_rate, "wrong size after expand");
  // printf("array max size: %d, content size: %ld\n", array->max, sizeof(array->contents));
  // printf("expected size: %ld\n", array->max * sizeof(void *));
  // mu_assert(sizeof(array->contents) == array->max * sizeof(void *), "expanded array content size is wrong");

  DArray_contract(array);
  mu_assert((unsigned int)array->max == array->expand_rate + 1, "wrong size after contract");
  // mu_assert(sizeof(array->contents) == array->max * sizeof(void *), "contracted array content size is wrong");

  DArray_contract(array);
  mu_assert((unsigned int)array->max == array->expand_rate + 1, "wrong size after contract");
  // mu_assert(sizeof(array->contents) == array->max * sizeof(void *), "contracted array content size is wrong");

  return NULL;
}

char *test_push_pop() {
  int i = 0;
  for(i = 0; i < 1000; i++) {
    int *val = DArray_new(array);
    *val = i * 333;
    DArray_push(array, val);
  }

  // expands 4 times by 300 each time
  mu_assert(array->max == 1201, "wrong max size");

  for(i = 999; i >= 0; i--) {
    int *val = DArray_pop(array);
    mu_assert(val != NULL, "shouldnt get a null");
    mu_assert(*val == i * 333, "wrong value");
    DArray_free(val);
  }

  // contracts back down to the minimum
  mu_assert((unsigned int)array->max == array->expand_rate + 2, "wrong size after popping");
  // mu_assert(sizeof(array->contents) == array->max * array->element_size, "popped array content size is wrong");

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_new);
  mu_run_test(test_set);
  mu_run_test(test_get);
  mu_run_test(test_remove);
  mu_run_test(test_expand_contract);
  mu_run_test(test_push_pop);
  mu_run_test(test_destroy);

  return NULL;
}

RUN_TESTS(all_tests);