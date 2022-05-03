#include "minunit.h"
#include <lcthw/heap.h>
#include <lcthw/bstrlib.h>

#define INITIAL_CAPACITY 300

typedef struct {
  int value;
} IntBox;

static IntBox test1 = {.value = 1};
static IntBox test2 = {.value = 2};
static IntBox test3 = {.value = 3};
static IntBox test4 = {.value = 4};
static IntBox test5 = {.value = 5};

static inline int intcmp(const int a, const int b) {
  return (a > b) - (a < b); // no branches and avoids overflows
}

static inline int intbox_compare(void *a, void *b) {
  return intcmp(((IntBox *)a)->value, ((IntBox *)b)->value);
}

char *test_create(void) {
  Heap *heap = Heap_create(minHeap, NULL, INITIAL_CAPACITY);
  mu_assert(heap != NULL, "Heap create failed");
  Heap_destroy(heap);

  heap = Heap_create(maxHeap, NULL, INITIAL_CAPACITY);
  mu_assert(heap != NULL, "Heap create failed");
  Heap_destroy(heap);

  heap = Heap_create(customHeap, intbox_compare, INITIAL_CAPACITY);
  mu_assert(heap != NULL, "Heap create failed");
  Heap_destroy(heap);

  // designed to fail
  heap = Heap_create(minHeap, intbox_compare, INITIAL_CAPACITY);
  mu_assert(heap == NULL, "shouldnt be able to make a min heap and specify a comparison function");

  return NULL;
}

char *test_heap_destroy(void) {
  Heap *heap = Heap_create(minHeap, NULL, INITIAL_CAPACITY);
  mu_assert(heap != NULL, "Heap create failed");

  Heap_destroy(heap);

  return NULL;
}

char *test_heap_insert(void) {
  Heap *heap = Heap_create(minHeap, NULL, INITIAL_CAPACITY);
  mu_assert(heap != NULL, "Heap create failed");

  Heap_insert(heap, &test3.value, &test3);
  mu_assert(Heap_count(heap) == 1, "Heap count should be 1");

  HeapNode *node = Heap_extract(heap);
  mu_assert(node != NULL, "extracted node was null");
  mu_assert(*(int *)node->key == 3, "got wrong value from extract min");

  Heap_insert(heap, &test2.value, &test2);
  Heap_insert(heap, &test1.value, &test1);
  node = Heap_extract(heap);
  mu_assert(node != NULL, "extracted node was null");
  mu_assert(*(int *)node->key == 1, "got wrong value from extract min");

  Heap_destroy(heap);

  return NULL;
}

char *test_extract_min(void) {
  Heap *heap = Heap_create(minHeap, NULL, INITIAL_CAPACITY);
  mu_assert(heap != NULL, "Heap create failed");

  HeapNode *node = Heap_extract(heap);
  mu_assert(node == NULL, "extracted node was not null");

  Heap_insert(heap, &test3.value, &test3);
  Heap_insert(heap, &test1.value, &test1);
  Heap_insert(heap, &test2.value, &test2);
  Heap_insert(heap, &test5.value, &test5);
  Heap_insert(heap, &test4.value, &test4);

  for(int i = 1; i <= 5; i++) {
    node = Heap_extract(heap);
    mu_assert(node != NULL, "extracted node was null");
    mu_assert(*(int *)node->key == i, "got wrong value from extract min");
  }

  mu_assert(Heap_count(heap) == 0, "heap size wasnt zero");

  Heap_destroy(heap);

  return NULL;
}

char *test_extract_min_inserted_ascending(void) {
  Heap *heap = Heap_create(minHeap, NULL, INITIAL_CAPACITY);
  mu_assert(heap != NULL, "Heap create failed");

  HeapNode *node = Heap_extract(heap);
  mu_assert(node == NULL, "extracted node was not null");

  Heap_insert(heap, &test1.value, &test1);
  Heap_insert(heap, &test2.value, &test2);
  Heap_insert(heap, &test3.value, &test3);
  Heap_insert(heap, &test4.value, &test4);
  Heap_insert(heap, &test5.value, &test5);

  for(int i = 1; i <= 5; i++) {
    node = Heap_extract(heap);
    mu_assert(node != NULL, "extracted node was null");
    mu_assert(*(int *)node->key == i, "got wrong value from extract min");
  }

  mu_assert(Heap_count(heap) == 0, "heap size wasnt zero");

  Heap_destroy(heap);

  return NULL;
}

char *test_extract_max(void) {
  Heap *heap = Heap_create(maxHeap, NULL, INITIAL_CAPACITY);
  mu_assert(heap != NULL, "Heap create failed");

  HeapNode *node = Heap_extract(heap);
  mu_assert(node == NULL, "extracted node was not null");

  Heap_insert(heap, &test3.value, &test3);
  Heap_insert(heap, &test1.value, &test1);
  Heap_insert(heap, &test2.value, &test2);
  Heap_insert(heap, &test5.value, &test5);
  Heap_insert(heap, &test4.value, &test4);

  for(int i = 5; i >= 1; i--) {
    node = Heap_extract(heap);
    mu_assert(node != NULL, "extracted node was null");
    mu_assert(*(int *)node->key == i, "got wrong value from extract max");
  }

  mu_assert(Heap_count(heap) == 0, "heap size wasnt zero");

  Heap_destroy(heap);

  return NULL;
}

char *test_extract_max_inserted_ascending(void) {
  Heap *heap = Heap_create(maxHeap, NULL, INITIAL_CAPACITY);
  mu_assert(heap != NULL, "Heap create failed");

  HeapNode *node = Heap_extract(heap);
  mu_assert(node == NULL, "extracted node was not null");

  Heap_insert(heap, &test1.value, &test1);
  Heap_insert(heap, &test2.value, &test2);
  Heap_insert(heap, &test3.value, &test3);
  Heap_insert(heap, &test4.value, &test4);
  Heap_insert(heap, &test5.value, &test5);

  for(int i = 5; i >= 1; i--) {
    node = Heap_extract(heap);
    mu_assert(node != NULL, "extracted node was null");
    mu_assert(*(int *)node->key == i, "got wrong value from extract max");
  }

  mu_assert(Heap_count(heap) == 0, "heap size wasnt zero");

  Heap_destroy(heap);

  return NULL;
}

char *test_peak_max(void) {
  Heap *heap = Heap_create(maxHeap, NULL, INITIAL_CAPACITY);
  mu_assert(heap != NULL, "Heap create failed");

  HeapNode *node = Heap_extract(heap);
  mu_assert(node == NULL, "extracted node was not null");

  Heap_insert(heap, &test3.value, &test3);
  Heap_insert(heap, &test1.value, &test1);
  Heap_insert(heap, &test2.value, &test2);
  Heap_insert(heap, &test5.value, &test5);
  Heap_insert(heap, &test4.value, &test4);

  for(int i = 5; i >= 1; i--) {
    node = Heap_peak(heap);
    mu_assert(node != NULL, "peaked node was null");
    mu_assert(*(int *)node->key == i, "got wrong value from peak max");

    Heap_extract(heap); // just doing this so the next peak will work
  }

  mu_assert(Heap_count(heap) == 0, "heap size wasnt zero");

  Heap_destroy(heap);

  return NULL;
}

char *all_tests(void) {
	mu_suite_start();

	mu_run_test(test_create);
	mu_run_test(test_heap_destroy);
  mu_run_test(test_heap_insert);
  mu_run_test(test_extract_min);
  mu_run_test(test_extract_min_inserted_ascending);
  mu_run_test(test_extract_max);
  mu_run_test(test_extract_max_inserted_ascending);
  mu_run_test(test_peak_max);

	return NULL;
}

RUN_TESTS(all_tests);