#ifndef _lcthw_hashmap_h
#define _lcthw_hashmap_h

#include <stdint.h>
#include <lcthw/darray.h>

#define DEFAULT_NUMBER_OF_BUCKETS 100

typedef int (*Hashmap_compare)(void *a, void *b);
typedef uint32_t(*Hashmap_hash)(void *key);

typedef struct Hashmap {
  size_t count;
  size_t num_buckets;
  DArray *buckets;
  Hashmap_compare compare;
  Hashmap_hash hash;
} Hashmap;

typedef struct HashmapNode {
  void *key;
  void *data;
  uint32_t hash;
} HashmapNode;

typedef int (*Hashmap_traverse_cb)(HashmapNode *node);

Hashmap *Hashmap_create(Hashmap_compare compare, Hashmap_hash hash);
Hashmap *Hashmap_create_buckets(Hashmap_compare compare, Hashmap_hash hash, size_t num_buckets);
void Hashmap_destroy(Hashmap *map);

int Hashmap_set(Hashmap *map, void *key, void *data);
void *Hashmap_get(Hashmap *map, void *key);

#define Hashmap_count(h) (h)->count

int Hashmap_traverse(Hashmap *map, Hashmap_traverse_cb traverse);

void *Hashmap_delete(Hashmap *map, void *key);

#endif