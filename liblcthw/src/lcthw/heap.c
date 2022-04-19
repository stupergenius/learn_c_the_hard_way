#include <lcthw/heap.h>
#include <stdbool.h>
#include "dbg.h"

// 1-based accessor helper methods. They do no bounds checking.
#define Heap_node_at(h, i) DArray_get((h)->nodes, (i)-1)
#define Heap_set_at(h, i, el) DArray_set((h)->nodes, (i)-1, el)

HeapNode *HeapNode_create(void *key, void *value) {
  check(key != NULL, "Key for a heap node cannot be null");

  HeapNode *node = malloc(sizeof(HeapNode));
  check_mem(node);

  node->key = key;
  node->value = value;

  return node;

  error:
    return NULL;
}

static inline int intcmp(const int a, const int b) {
  return (a > b) - (a < b); // no branches and avoids overflows
}

static inline int default_min_heap_compare(void *a, void *b) {
  return intcmp(*(int *)a, *(int *)b);
}

static inline int default_max_heap_compare(void *a, void *b) {
  return intcmp(*(int *)a, *(int *)b);
}

static inline Heap_key_compare default_heap_compare(HeapType type) {
  return type == minHeap
    ? default_min_heap_compare
    : default_max_heap_compare;
}

Heap *Heap_create(HeapType type, Heap_key_compare compare, size_t initial_capacity) {
  Heap *heap = NULL;
  check(type == customHeap || compare == NULL, "Heap comparator can only be given for `customHeap` types");

  heap = malloc(sizeof(Heap));
  check_mem(heap);

  heap->nodes = DArray_create(sizeof(HeapNode *), initial_capacity);
  check_mem(heap->nodes);

  heap->type = type;
  heap->compare = compare != NULL ? compare : default_heap_compare(type);
  heap->count = 0;

  return heap;

  error:
    if (heap != NULL && heap->nodes != NULL) DArray_clear_destroy(heap->nodes);
    if (heap != NULL) free(heap);
    return NULL;
}

static void Heap_bubble_last_up(Heap *heap) {
  check(heap != NULL, "Heap parameter cannot be null");

  // return early if this is a heap of size 1
  if (Heap_count(heap) <= 1) return;

  int inserted_index = Heap_count(heap);
  int parent_index = inserted_index / 2; // will floor by default so no need to check even/odd

  HeapNode *parent_node = Heap_node_at(heap, parent_index);
  HeapNode *bubbling_node = Heap_node_at(heap, inserted_index);

  while (inserted_index != 1 && heap->compare(parent_node->key, bubbling_node->key) > 0) {
    Heap_set_at(heap, inserted_index, parent_node);
    Heap_set_at(heap, parent_index, bubbling_node);

    inserted_index = parent_index;
    parent_index = parent_index / 2; // As above, will floor
    parent_node = Heap_node_at(heap, parent_index);
  }

  error:
    return;
}

void Heap_insert(Heap *heap, void *key, void *value) {
  check(heap != NULL && key != NULL, "Heap and key parameters cannot be null");

  HeapNode *node = HeapNode_create(key, value);
  check(node != NULL, "Failed to create heap node with key=%p", key);

  DArray_push(heap->nodes, node);
  heap->count++;

  Heap_bubble_last_up(heap);

  error:
    return;
}

static void Heap_bubble_root_down(Heap *heap) {
  check(heap != NULL, "Heap parameter cannot be null");

  // return early if this is a heap of size 1
  size_t heap_count = Heap_count(heap);
  if (heap_count <= 1) return;

  int bubbling_index = 1; // bubble down always starts at the root
  size_t left_child_index = 2 * bubbling_index;
  size_t right_child_index = left_child_index + 1;

  HeapNode *bubbling_node = Heap_node_at(heap, bubbling_index);
  HeapNode *left_node = NULL;
  HeapNode *right_node = NULL;
  bool left_property = false;
  bool right_property = false;
  size_t swap_index = left_child_index;
  HeapNode *swap_node = NULL;

  // The heap count comparison allows the loop to terminate at the "bottom" of a sub-tree,
  // since if we've swapped to a node with no children... the heap property is preserved by definition.
  while (left_child_index < heap_count) {
    left_node = Heap_node_at(heap, left_child_index);
    right_node = right_child_index < heap_count ? Heap_node_at(heap, right_child_index) : NULL;

    // if the heap property is met for both left and right nodes (when present)
    left_property = heap->compare(left_node->key, bubbling_node->key) > 0;
    right_property = right_node == NULL || heap->compare(right_node->key, bubbling_node->key) > 0;
    if (left_property && right_property) {
      // then we're done!
      break;
    }

    // otherwise need to continue down, swapping along the way
    swap_index = left_child_index;
    swap_node = left_node;

    // no need to compare if there's only one node to swap with
    // otherwise we swap with the "smaller" node
    if (right_node != NULL && heap->compare(left_node->key, right_node->key) > 0) {
      swap_index = right_child_index;
      swap_node = right_node;
    }

    // do the swap
    Heap_set_at(heap, swap_index, bubbling_node);
    Heap_set_at(heap, bubbling_index, swap_node);
    bubbling_index = swap_index;

    // Update children indices for the next iteration based
    // on the new bubbling_index.
    left_child_index = 2 * bubbling_index;
    right_child_index = left_child_index + 1;
  }

  error:
    return;
}

HeapNode *Heap_extract(Heap *heap) {
  check(heap != NULL, "Heap parameter cannot be null");

  HeapNode *extracted = NULL;

  if (Heap_count(heap) > 0) {
    // pull out the first node and replace it with the last node
    extracted = Heap_node_at(heap, 1);
    HeapNode *last_node = DArray_pop(heap->nodes);
    Heap_set_at(heap, 1, last_node);

    // decrement heap count so the bubble down method knows the new size
    heap->count--;

    // now bubble down
    Heap_bubble_root_down(heap);
  }

  return extracted;

  error:
    return NULL;
}

void Heap_destroy(Heap *heap) {
  if (heap == NULL) return;

  DArray_clear_destroy(heap->nodes);
  free(heap);
}