#ifndef _lcthw_darray_h
#define _lcthw_darray_h

#include <stdlib.h>
#include <assert.h>

typedef struct DArray {
  int end;
  int max;
  size_t element_size;
  size_t expand_rate;
  void **contents;
} DArray;

DArray *DArray_create(size_t element_size, size_t initial_max);

void DArray_set(DArray *array, int i, void *el);
void *DArray_get(DArray *array, int i);
void *DArray_remove(DArray *array, int i);
void *DArray_new(DArray *array);

void DArray_destroy(DArray *array);
void DArray_clear(DArray *array);
void DArray_clear_destroy(DArray *array);
int DArray_expand(DArray *array);
int DArray_contract(DArray *array);
int DArray_push(DArray *array, void *el);
void *DArray_pop(DArray *array);

#define DArray_last(A) ((A)->contents[(A)->end - 1])
#define DArray_first(A) ((A)->contents[(A)->start])
#define DArray_end(A) ((A)->end)
#define DArray_count(A) DArray_end(A)
#define DArray_max(A) ((A)->max)

#define DArray_free(E) free((E))

#define DEFAULT_EXPAND_RATE 300

#endif