#ifndef _lcthw_heap_h
#define _lcthw_heap_h

#include <stdlib.h>
#include <lcthw/darray.h>

typedef int (*Heap_key_compare)(void *a, void *b);

typedef struct HeapNode {
  void *key;
  void *value;
} HeapNode;

typedef enum HeapType {
  minHeap=1,
  maxHeap=2,
  customHeap=3
} HeapType;

typedef struct Heap {
  DArray *nodes; // contains HeapNodes
  size_t count;
  HeapType type;
  Heap_key_compare compare;
} Heap;

/*
* type - a HeapType value
* compare - A Heap_key_compare callback function to use for comparing HeapNode keys. Must be NULL unless type==customHeap. Using the default min/max types, the implementation assumes the keys inserted into the heap are int pointers.
* initial_capacity - An initial capacity for the heap. Wrong guesses will incur some performance penalty (due to being implemented with a resizing dynamic array) after the initial capacity is exhausted.
*/
Heap *Heap_create(HeapType type, Heap_key_compare compare, size_t initial_capacity);

#define Heap_count(h) (h)->count

// An O(logn) insert operation
void Heap_insert(Heap *heap, void *key, void *value);

// An O(logn) operation to retrieve and remove the min/max element out of the heap (based on what "type" it is).
HeapNode *Heap_extract(Heap *heap);

// An O(logn) operation to retrieve (but not remove) the min/max element out of the heap (based on what "type" it is).
HeapNode *Heap_peak(Heap *heap);

// finds the rank (aka a 1-based index) of the given key in the heap, or 0 if not found
size_t Heap_index_of(Heap *heap, void *key);

// Assumes the caller owns the HeapNode keys and values and won't free them.
void Heap_destroy(Heap *heap);

#endif