//
// Created by x1314aq on 2019/1/29.
//

#include "avl.h"
#include "list.h"
#include "mempool.h"


struct map_entry {
    struct avl_node node;
    uint64_t hash;
    void *key;
    void *val;
};

struct map_bucket {
    struct avl_root root;
    struct list_entry entry;       /// point to next/prev non-empty map_bucket
};

struct map_type {
    uint64_t (*hash_func)(const void *key);
    int (*key_compare)(const void *key1, const void *key2);
};

struct map {
    uint64_t size;                 /// power of 2, number of map_bucket
    uint64_t mask;                 /// size - 1
    uint64_t used;                 /// number of entries we have
    struct map_type type;          /// operations
    struct map_bucket *buckets;    /// buckets/slots
    struct list_entry head;        /// point to first/last non-empty map_bucket
    struct mempool mp;             /// mempool used for fast alloc/dealloc
};

int
avl_map_init(struct map *map, struct map_type *type);

void
avl_map_destroy(struct map *map);

struct map_entry *
avl_map_first(struct map *map);

struct map_entry *
avl_map_last(struct map *map);

struct map_entry *
avl_map_next(struct map *map, struct map_entry *entry);

struct map_entry *
avl_map_prev(struct map *map, struct map_entry *entry);

struct map_entry *
avl_map_find(struct map *map, const void *key);

/**
 * Insert K-V pair to map
 * Do not allow duplicate entry to be inserted
 *
 * @return
 *   0: success
 *  -1: fail, duplicate entry was found
 */
int
avl_map_insert(struct map *map, void *key, void *value);

/**
 * Erase K-V pair from map
 * Call avl_map_find() before erasing because we expect the entry does exactly exist
 */
void
avl_map_erase(struct map *map, struct map_entry *entry);
