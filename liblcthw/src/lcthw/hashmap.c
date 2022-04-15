#include <stdint.h>
#include <lcthw/hashmap.h>
#include <lcthw/dbg.h>
#include <lcthw/bstrlib.h>

// Default implementations for compare and hash

static int default_compare(void *a, void *b) {
  return bstrcmp((bstring) a, (bstring) b);
}

/**
 * Simple Bob Jenkin's hash algorithm taken from Wikipedia description
 */
static uint32_t default_hash(void *a) {
  size_t len = blength((bstring) a);
  char *key = bdata((bstring) a);
  uint32_t hash = 0;
  uint32_t i = 0;

  for(hash = i = 0; i < len; i++) {
    hash += key[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return hash;
}

Hashmap *Hashmap_create_buckets(Hashmap_compare compare, Hashmap_hash hash, size_t num_buckets) {
  check(num_buckets > 0, "hash buckets must be > 0");

  Hashmap *map = calloc(1, sizeof(Hashmap));
  check_mem(map);

  map->count = 0;
  map->num_buckets = num_buckets;
  map->compare = compare == NULL ? default_compare : compare;
  map->hash = hash == NULL ? default_hash : hash;

  map->buckets = DArray_create(sizeof(DArray *), num_buckets);
  check_mem(map->buckets);

  // fake it so that it thinks its "full"
  map->buckets->end = map->buckets->max;

  return map;
  error:
    return NULL;
}

Hashmap *Hashmap_create(Hashmap_compare compare, Hashmap_hash hash) {
  return Hashmap_create_buckets(compare, hash, DEFAULT_NUMBER_OF_BUCKETS);
}

// Hashmap and HashmapNode lifecycle functions

void Hashmap_destroy(Hashmap *map) {
  if (!map) return;

  if (map->buckets) {
    for(int i = 0; i < DArray_count(map->buckets); i++) {
      DArray *bucket = DArray_get(map->buckets, i);
      if (bucket) DArray_clear_destroy(bucket);
    }
    DArray_destroy(map->buckets);
  }

  free(map);
}

static inline HashmapNode *Hashmap_node_create(uint32_t hash, void *key, void *data) {
  HashmapNode *node = calloc(1, sizeof(HashmapNode));
  check_mem(node);

  node->key = key;
  node->hash = hash;
  node->data = data;

  return node;
  error:
    return NULL;
}

// Internal bucket methods

static inline DArray *Hashmap_find_bucket(Hashmap *map, void *key, int create, uint32_t *hash_out) {
  uint32_t hash = map->hash(key);
  int bucket_n = hash % (map->num_buckets);
  check(bucket_n >= 0, "invalid bucket found: %d", bucket_n);

  // store the hash into the _out_ parameter
  *hash_out = hash;

  DArray *bucket = DArray_get(map->buckets, bucket_n);
  if (!bucket && create) {
    bucket = DArray_create(sizeof(Hashmap *), map->num_buckets);
    check_mem(bucket);
    DArray_set(map->buckets, bucket_n, bucket);
  }

  return bucket;
  error:
    return NULL;
}

static inline int Hashmap_find_node_index(Hashmap *map, DArray *bucket, void *key, uint32_t hash) {
  HashmapNode *node = NULL;
  for(int i = 0; i < DArray_end(bucket); i++) {
    node = DArray_get(bucket, i);
    // short-circuit means we only do the "expensive" compare when the hash matches first
    if (node && node->hash == hash && map->compare(node->key, key) == 0) {
      return i;
    }
  }
  return -1;
}

static inline HashmapNode *Hashmap_find_node(Hashmap *map, DArray *bucket, void *key, uint32_t hash) {
  int index = Hashmap_find_node_index(map, bucket, key, hash);
  return index >= 0 ? DArray_get(bucket, index) : NULL;
}

// Hashmap accessors

int Hashmap_set(Hashmap *map, void *key, void *data) {
  uint32_t hash = 0;
  DArray *bucket = Hashmap_find_bucket(map, key, 1, &hash);
  check(bucket != NULL, "Can't find/create bucket");

  HashmapNode *node = Hashmap_node_create(hash, key, data);
  check(node != NULL, "Create node failed");

  DArray_push(bucket, node);
  map->count++;

  return 0;
  error:
    return -1;
}

void *Hashmap_get(Hashmap *map, void *key) {
  uint32_t hash = 0;
  DArray *bucket = Hashmap_find_bucket(map, key, 0, &hash);
  check_debug(bucket != NULL, "Can't find bucket for key=%p hash=%d", key, hash);

  HashmapNode *node = Hashmap_find_node(map, bucket, key, hash);
  if (node) return node->data;

  error:
    return NULL;
}

int Hashmap_traverse(Hashmap *map, Hashmap_traverse_cb traverse) {
  if (!map) return -1;

  for(int i = 0; i < DArray_count(map->buckets); i++) {
    DArray *bucket = DArray_get(map->buckets, i);
    if (!bucket) continue;

    for(int j = 0; j < DArray_count(bucket); j++) {
      HashmapNode *node = DArray_get(bucket, j);
      int rc = traverse(node);
      if (rc != 0) return rc;
    }
  }

  return 0;
}

void *Hashmap_delete(Hashmap *map, void *key) {
  uint32_t hash = 0;
  DArray *bucket = Hashmap_find_bucket(map, key, 0, &hash);
  if (!bucket) return NULL;

  int index = Hashmap_find_node_index(map, bucket, key, hash);
  if (index < 0) return NULL;

  HashmapNode *node = DArray_get(bucket, index);
  if (!node) return NULL;

  HashmapNode *ending = DArray_pop(bucket);
  if (node != ending) {
    // to keep the array from becoming "sparse", we swap
    // the current "end" with the node matching the given key,
    // effectively removing it from the bucket array
    DArray_set(bucket, index, ending);
  }
  map->count--;

  void *val = node->data;
  free(node);

  return val;
}
