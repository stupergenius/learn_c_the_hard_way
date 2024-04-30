#ifndef _lcthw_union_find_h
#define _lcthw_union_find_h

#include <stdlib.h>
#include <stdbool.h>

typedef struct UnionFind UnionFind;
typedef struct UnionFind_Node;

UnionFind *UnionFind_create();
UnionFind *UnionFind_create_initialized(void **elements, size_t cardinality);
void UnionFind_destroy(UnionFind *uf);

void UnionFind_make_set(UnionFind *uf, void *el);

// returns the number of groups of sets
size_t UnionFind_cardinality(UnionFind *uf);

// returns the root of the given element's group (which could be el itself), or NULL if el is not in the set
void *UnionFind_find(UnionFind *uf, void *el);

// returns the size of the subset itself
size_t UnionFind_Node_cardinality(UnionFind_Node *n);

// returns true when the given elemnents a and b share a root
bool UnionFind_Node_in_same_group(UnionFind *uf, void *a, void *b);

// unions the two
void UnionFind_union(UnionFind *uf, void *a, void *b);

#endif