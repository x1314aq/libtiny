//
// Created by xaq on 10/9/17.
//

#include "map.h"
#include <stdlib.h>
#include <limits.h>

/* -------------------------- private prototypes ---------------------------- */
static int _map_init(map *m, map_type *type);

static void _map_reset(map_ht *ht);

static map_entry *_map_put_base(map *m, uint64_t key, map_entry **existing);

static map_entry *_map_find(map *m, uint64_t key);

static map_entry *_map_rm_base(map *m, uint64_t key);

static int _map_clear(map *m, map_ht *ht);

static void _map_rehash(map *m);

static int _map_key_index(map *m, uint64_t key, map_entry **existing);

static int _map_expand_if_needed(map *m);

static int _map_expand(map *m, unsigned long size);

static unsigned long _map_next_power(unsigned long size);

/* ----------------------------- API implementation ------------------------- */
map *map_create(map_type *type){
    map *m = malloc(sizeof(map));
    _map_init(m, type);
    return m;
}

int map_put(map *m, uint64_t key, void *val){
    map_entry *entry = _map_put_base(m, key, NULL);

    if(!entry) return ERROR;
    if(m->type->value_dup)
        entry->v.val = m->type->value_dup(val);
    else
        entry->v.val = val;
    return SUCC;
}

void *map_get(map *m, uint64_t key){
    map_entry *entry = _map_find(m, key);
    return entry ? entry->v.val : NULL;
}

int map_rm(map *m, uint64_t key){
    return _map_rm_base(m, key) ? SUCC : ERROR;
}

void map_free(map *m){
    _map_clear(m, &m->ht[0]);
    _map_clear(m, &m->ht[1]);
    free(m);
}

map_entry *map_find(map *m, uint64_t key){
    return _map_find(m, key);
}

uint64_t _default_int_hash_func(uint32_t key){
    key += ~(key << 15);
    key ^= (key >> 10);
    key += (key << 3);
    key ^= (key >> 6);
    key += ~(key << 11);
    key ^= (key >> 16);
    return key;
}

/* Murmur hash function 64bit version */
uint64_t _default_string_has_func(const void *key, int len, uint64_t seed){
    const uint64_t m = 0xc6a4a7935bd1e995ULL;
    const int r = 47;

    uint64_t h = seed ^(len * m);

    const uint64_t *data = (const uint64_t *) key;
    const uint64_t *end = data + (len / 8);

    while(data != end){
        uint64_t k = *data++;

        k *= m;
        k ^= k >> r;
        k *= m;

        h ^= k;
        h *= m;
    }

    const unsigned char *data2 = (const unsigned char *) data;

    switch(len & 7){
        case 7:
            h ^= (uint64_t) (data2[6]) << 48;
        case 6:
            h ^= (uint64_t) (data2[5]) << 40;
        case 5:
            h ^= (uint64_t) (data2[4]) << 32;
        case 4:
            h ^= (uint64_t) (data2[3]) << 24;
        case 3:
            h ^= (uint64_t) (data2[2]) << 16;
        case 2:
            h ^= (uint64_t) (data2[1]) << 8;
        case 1:
            h ^= (uint64_t) (data2[0]);
            h *= m;
    };

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
}

/* ------------------------ private API implementation ---------------------- */
int _map_init(map *m, map_type *type){
    _map_reset(&m->ht[0]);
    _map_reset(&m->ht[1]);
    m->table = 0;
    m->type = type;
    return SUCC;
}

void _map_reset(map_ht *ht){
    ht->buckets = NULL;
    ht->size = 0;
    ht->size_mask = 0;
    ht->used = 0;
}

int _map_clear(map *m, map_ht *ht){
    unsigned long i;

    for(i = 0; i < ht->size && ht->used > 0; i++){
        map_entry *entry, *next_entry;
        if((entry = ht->buckets[i]) == NULL) continue;
        while(entry){
            next_entry = entry->next;
            if(m->type->value_free)
                m->type->value_free(entry->v.val);
            free(entry);
            ht->used--;
            entry = next_entry;
        }
    }
    free(ht->buckets);
    _map_reset(ht);
    return SUCC;
}

map_entry *_map_put_base(map *m, uint64_t key, map_entry **existing){
    int index;
    map_entry *entry;
    map_ht *ht;

    if((index = _map_key_index(m, key, existing)) == -1)
        return NULL;

    ht = &m->ht[m->table];
    entry = malloc(sizeof(map_entry));
    entry->next = ht->buckets[index];
    ht->buckets[index] = entry;
    ht->used++;
    entry->key = key;
    return entry;
}

int _map_key_index(map *m, const uint64_t key, map_entry **existing){
    unsigned int hash, index;
    int table;
    map_entry *entry;

    hash = m->type->hash_func(&key);
    if(existing) *existing = NULL;

    if(_map_expand_if_needed(m) == ERROR)
        return -1;

    table = m->table;
    index = hash & m->ht[table].size_mask;
    entry = m->ht[table].buckets[index];
    while(entry){
        if((m->type->key_compare && m->type->key_compare(key, entry->key) == 0) || key == entry->key){
            if(existing) *existing = entry;
            return -1;
        }
        entry = entry->next;
    }

    return index;
}

int _map_expand_if_needed(map *m){
    int table;
    table = m->table;

    if(m->ht[table].size == 0) return _map_expand(m, MAP_HT_INITIAL_SIZE);

    if(m->ht[table].used >= m->ht[table].size) return _map_expand(m, m->ht[table].used * 2);

    return SUCC;
}

int _map_expand(map *m, unsigned long size){
    map_ht new_ht;
    int table, new_table;
    unsigned long real_size;

    table = m->table;
    real_size = _map_next_power(size);

    if(real_size == m->ht[table].size) return ERROR;

    new_ht.size = real_size;
    new_ht.size_mask = real_size - 1;
    new_ht.buckets = calloc(real_size, sizeof(map_entry *));
    new_ht.used = 0;

    new_table = (~table) & 1;

    m->ht[new_table] = new_ht;
    _map_rehash(m);
    return SUCC;
}

unsigned long _map_next_power(unsigned long size){
    unsigned long i = MAP_HT_INITIAL_SIZE;

    if(size >= LONG_MAX) return LONG_MAX;
    while(1){
        if(i >= size)
            return i;
        i *= 2;
    }
}

map_entry *_map_find(map *m, uint64_t key){
    map_entry *entry;
    unsigned int hash, index;
    int table;

    if(m->ht[0].used + m->ht[1].used == 0) return NULL;

    hash = m->type->hash_func(&key);
    table = m->table;
    index = hash & m->ht[table].size_mask;
    entry = m->ht[table].buckets[index];
    while(entry){
        if((m->type->key_compare && m->type->key_compare(key, entry->key) == 0) || key == entry->key)
            return entry;
        entry = entry->next;
    }
    return NULL;
}

map_entry *_map_rm_base(map *m, uint64_t key){
    unsigned int hash, index;
    int table;
    map_entry *entry, *prev_entry;

    table = m->table;
    if(m->ht[0].used + m->ht[1].used == 0) return NULL;

    hash = m->type->hash_func(&key);
    index = hash & m->ht[table].size_mask;
    entry = m->ht[table].buckets[index];
    prev_entry = NULL;
    while(entry){
        if((m->type->key_compare && m->type->key_compare(key, entry->key) == 0) || key == entry->key){
            if(prev_entry)
                prev_entry->next = entry->next;
            else
                m->ht[table].buckets[index] = entry->next;
            if(m->type->value_free)
                m->type->value_free(entry->v.val);
            free(entry);
            m->ht[table].used--;
            return entry;
        }
        prev_entry = entry;
        entry = entry->next;
    }
    return NULL;
}

void _map_rehash(map *m){
    unsigned int hash, index;
    int table, new_table;
    unsigned long i;

    table = m->table;
    new_table = (~table) & 1;
    for(i = 0; i < m->ht[table].size; i++){
        map_entry *entry, *next_entry;
        if((entry = m->ht[table].buckets[i]) != NULL){
            while(entry){
                next_entry = entry->next;
                entry->next = NULL;
                hash = m->type->hash_func(&entry->key);
                index = hash & m->ht[new_table].size_mask;
                if(m->ht[new_table].buckets[index]){
                    entry->next = m->ht[new_table].buckets[index];
                    m->ht[new_table].buckets[index] = entry;
                } else
                    m->ht[new_table].buckets[index] = entry;
                entry = next_entry;
            }
        }
    }
    m->table = new_table;
    m->ht[new_table].used = m->ht[table].used;
    free(m->ht[table].buckets);
    _map_reset(&m->ht[table]);
}