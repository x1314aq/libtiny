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
} map_type;

typedef struct map {
    map_type *type;
    map_ht ht[2];
    int table;
} map;

#define MAP_HT_INITIAL_SIZE    4

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

uint64_t _murmurHash64A(const void *key, int len, uint64_t seed);

uint64_t _default_int_hash_func(uint32_t key);

#ifdef __cplusplus
}
#endif

#endif //STL_MAP_H