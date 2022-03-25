#include <lcthw/darray.h>
#include <lcthw/dbg.h>

void DArray_set(DArray *array, int i, void *el) {
  check(i < array->max, "attempt to set past max");

  if (i > array->end) {
    array->end = i;
  }
  array->contents[i] = el;

  error:
    return;
}

void *DArray_get(DArray *array, int i) {
  check(i < array->max, "attempt to get past max");

  return array->contents[i];

  error:
    return NULL;
}

void *DArray_remove(DArray *array, int i) {
  void *el = NULL;
  check(i < array->max, "attempt to remove past max");

  el = array->contents[i];
  array->contents[i] = NULL;

  error:
    return el;
}

void *DArray_new(DArray *array) {
  check(array->element_size > 0, "called on 0 sized array");

  return calloc(1, array->element_size);

  error:
    return NULL;
}

DArray *DArray_create(size_t element_size, size_t initial_max) {
  check(element_size > 0, "must give a >0 value for element size");
  check(initial_max > 0, "must give a >0 value for initial max");

  DArray *array = malloc(sizeof(DArray));
  check_mem(array);

  array->contents = calloc(initial_max, sizeof(void *));
  check_mem(array->contents);

  array->end = 0;
  array->element_size = element_size;
  array->max = initial_max;
  array->expand_rate = DEFAULT_EXPAND_RATE;

  return array;

  error:
    return NULL;
}

void DArray_destroy(DArray *array) {
  free(array->contents);
  free(array);
}

void DArray_clear(DArray *array) {
  if (array->element_size <= 0 || array->end <= 0) return;

  for(int i = 0; i < array->max; i++) {
    if (array->contents[i] == NULL) continue;

    free(array->contents[i]);
    array->contents[i] = NULL;
  }

  array->end = 0;
}

void DArray_clear_destroy(DArray *array) {
  DArray_clear(array);
  DArray_destroy(array);
}

static inline int DArray_resize(DArray *array, size_t new_size) {
  check(new_size > 0, "new size must be > 0");

  void *new_contents = realloc(array->contents, new_size * sizeof(void *));
  check_mem(new_contents);

  array->max = new_size;
  array->contents = new_contents;

  return 0;
  error:
    return -1;
}

int DArray_expand(DArray *array) {
  check(array->expand_rate > 0, "expand rate should be > 0");
  check(array->element_size > 0, "element size should be > 0");

  size_t old_max = array->max;
  // expand up in intervals of expand_rate
  size_t new_size = array->max + array->expand_rate;

  int rc = DArray_resize(array, new_size);
  check(rc == 0, "failed to expand array to new size: %d", (int)new_size);

  // set the new memory - from the old "end" of the array to the new expanded size - to zeros
  memset(array->contents + old_max, 0, array->expand_rate + 1);

  return 0;
  error:
    return -1;
}

int DArray_contract(DArray *array) {
  check(array->expand_rate > 0, "expand rate should be > 0");
  check(array->element_size > 0, "element size should be > 0");

  // contract down to the greater of the array's end or the expand rate
  size_t new_size = array->end < (int)array->expand_rate ? (int)array->expand_rate : array->end;
  return DArray_resize(array, new_size + 1); // i dont know why there's a +1 here

  error:
    return -1;
}

int DArray_push(DArray *array, void *el) {
  array->contents[array->end] = el;
  array->end += 1;

  // pre-emptively expand for the next push
  if (array->end == array->max) {
    check(DArray_expand(array) == 0, "failed to expand array");
  }

  return 0;
  error:
    return -1;
}

void *DArray_pop(DArray *array) {
  if (array->end <= 0) return NULL;

  void *el = array->contents[array->end - 1];
  array->end -= 1;

  // contract when we're a multiple of expand_rate bigger than the number of elements in the array
  if (array->end > (int)array->expand_rate && array->end % array->expand_rate) {
    DArray_contract(array);
  }

  return el;
}