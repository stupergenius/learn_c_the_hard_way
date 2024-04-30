#include <lcthw/union_find.h>
#include "dbg.h"

typedef struct UnionFind_Node {
  void *el;
  UnionFind_Node *parent;
  size_t size;
} UnionFind_Node;

typedef struct UnionFind {
  size_t size;
} UnionFind;

UnionFind *UnionFind_create() {
  return NULL;
}

UnionFind *UnionFind_create_initialized(void **elements, size_t cardinality) {
  return NULL;
}

void UnionFind_destroy(UnionFind *uf) {

}

void UnionFind_make_set(UnionFind *uf, void *el) {

}

// returns the number of groups of sets
size_t UnionFind_cardinality(UnionFind *uf) {
  return 0;
}

// returns the "name" of the element or NULL if not found
void *UnionFind_find(UnionFind *uf, void *el) {
  return NULL;
}

void UnionFind_union(UnionFind *uf, void *a, void *b) {

}
