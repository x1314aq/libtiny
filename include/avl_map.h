//
// Created by x1314aq on 2019/1/29.
//

#include "avl.h"
#include "list.h"


struct map_entry {
    struct avl_node node;
    uint64_t hash;
    void *key;
    union {
        void *val;
        uint64_t u64;
        int64_t s64;
    } v;
};

struct map_bucket {
    struct avl_root root;
    struct list_entry entry;       /// point to next/prev non-empty map_bucket
};

struct map_type {
    uint64_t (*hash_func)(const void *key);
    int (*key_compare)(uint64_t key1, uint64_t key2);
};

struct map {
    uint64_t size;                 /// power of 2, number of map_bucket
    uint64_t mask;                 /// size - 1
    uint64_t used;                 /// number of entries we have
    struct map_type *type;         /// operations
    struct map_bucket *buckets;    /// buckets/slots
    struct list_entry head;        /// point to first/last non-empty map_bucket
};

void avl_map_init(struct map *map, struct map_type *type);

struct map_entry *avl_map_first(struct map *map);
struct map_entry *avl_map_last(struct map *map);
struct map_entry *avl_map_next(struct map *map, struct map_entry *entry);
struct map_entry *avl_map_prev(struct map *map, struct map_entry *entry);

struct map_entry *avl_map_find(struct map *map, const void *key);

/**
 * Do not allow duplicate nodes to be inserted
 *
 * @return
 *   0: success
 *  -1: fail
 */
int avl_map_insert(struct map *map, struct map_entry *entry);

/**
 * Erase K-V pair from map
 */
void avl_map_erase(struct map *map, struct map_entry *entry);
