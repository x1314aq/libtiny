//
// Created by xaq on 10/9/17.
//

#include <stdint.h>
#include <stddef.h>

#ifndef STL_MAP_H
#define STL_MAP_H

typedef struct map_entry {
    uint64_t key;
    union {
        void *val;
        uint64_t u64;
        int64_t s64;
        double d;
    } v;
    struct map_entry *next;
} map_entry;

typedef struct map_ht {
    map_entry **buckets;
    unsigned long size;
    unsigned long size_mask;
    unsigned long used;
} map_ht;

typedef struct map_type {
    uint64_t (*hash_func)(const void *key);

    void *(*value_dup)(const void *obj);

    void (*value_free)(void *val);

    int (*key_compare)(uint64_t key1, uint64_t key2);
} map_type;

typedef struct map {
    map_type *type;
    map_ht ht[2];    /* ht[0]存放key小于128的键值对，并且不做hash，直接访问 */
} map;

#define MAP_HT0_INITIAL_SIZE   128
#define MAP_HT1_INITIAL_SIZE   16
#define MAP_DIRECT_ACCESS      128
#define MAP_TREEIFY_THRESHOLD  8

#define SUCC    0
#define ERROR   1

/* API */

#ifdef __cplusplus
extern "C" {
#endif

map *map_create(map_type *type);

int map_put(map *m, uint64_t key, void *val);

int map_rm(map *m, uint64_t key);

void *map_get(map *m, uint64_t key);

void map_free(map *m);

map_entry *map_find(map *m, uint64_t key);

uint64_t _default_string_has_func(const void *key, int len, uint64_t seed);

uint64_t _default_int_hash_func(uint32_t key);

#ifdef __cplusplus
}
#endif

#endif //STL_MAP_H