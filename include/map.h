//
// Created by xaq on 10/9/17.
//

#include <stdint.h>
#include <stddef.h>

#ifndef STL_MAP_H
#define STL_MAP_H

typedef struct map_entry
{
    uint64_t key;
    union
    {
        void *val;
        uint64_t u64;
        int64_t s64;
        double d;
    } v;
    struct map_entry *next;
} map_entry;

typedef struct map_ht
{
    map_entry **buckets;
    unsigned long size;
    unsigned long size_mask;
    unsigned long used;
} map_ht;

typedef struct map_type
{
    uint64_t (*hash_func)(const void *key);

    void *(*value_dup)(const void *obj);

    void (*value_free)(void *val);

    int (*key_compare)(uint64_t key1, uint64_t key2);
} map_type;

/* *************************************************
 * ht[0]存放key小于128的键值对，并且不做hash，直接访问
 * ht[1]正常的通过hash函数访问
 * **************************************************/
typedef struct map
{
    map_type *type;
    map_ht ht[2];
} map;

typedef struct map_iterator
{
    map *m;                /* 当前迭代的map */
    long index;            /* 当前迭代的buckets数组下标索引 */
    int table;             /* 当前迭代的table */
    map_entry *entry;      /* 当前节点 */
    map_entry *next_entry; /* 当前节点的后继节点 */
} map_iterator;

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

int map_put_u64(map *m, uint64_t key, uint64_t val);

int map_put_s64(map *m, uint64_t key, int64_t val);

int map_rm(map *m, uint64_t key);

void *map_get(map *m, uint64_t key);

void map_free(map *m);

map_entry *map_find(map *m, uint64_t key);

uint64_t _default_string_has_func(const void *key, int len, uint64_t seed);

uint64_t _default_int_hash_func(uint32_t key);

map_iterator *map_get_iter(map *m);

map_entry *map_iter_next(map_iterator *iter);

void map_release_iter(map_iterator *iter);

#ifdef __cplusplus
}
#endif

#endif //STL_MAP_H