//
//  Created by x1314aq on 2019/2/24
//

#ifndef _LIBTINY_HASH_H_
#define _LIBTINY_HASH_H_

#include "common.h"

/**
 * Bitmap implementation using macros
 * Two bits are used to indicate slot's state,
 * higher bit for empty detection while lower
 * bit for lazy delete.
 */

/**
 * test whether given slot has been marked as deleted
 */
#define __is_delete(bm, i)    ((bm)[(i) >> 2] & (1U << (((i) & 0x3U) << 1)))

/**
 * test whether given slot has not been used
 */
#define __is_empty(bm, i)    ((bm)[(i) >> 2] & (2U << (((i) & 0x3U) << 1)))

/**
 * test whether given slot is empty or has been deleted
 */
#define __is_either(bm, i)    ((bm)[(i) >> 2] & (3U << (((i) & 0x3U) << 1)))

/**
 * test whether given slot has valid key-value pair (not empty and not deleted)
 */
#define __is_valid(bm, i)     !__is_either(bm, i)

/**
 * mark given slot as deleted
 */
#define __set_delete(bm, i)    ((bm)[(i) >> 2] |= (1U << (((i) & 0x3U) << 1)))

/**
 * mark given slot as empty
 */
#define __set_empty(bm, i)    ((bm)[(i) >> 2] |= (2U << (((i) & 0x3U) << 1)))

/**
 * clear delete mark of given slot
 */
#define __clear_delete(bm, i)   ((bm)[(i) >> 2] &= ~(1U << (((i) & 0x3U) << 1))) 

/**
 * clear empty mark of given slot
 */
#define __clear_empty(bm, i)   ((bm)[(i) >> 2] &= ~(2U << (((i) & 0x3U) << 1))) 

/**
 * clear both empty and delete mark of given slot
 */
#define __clear_both(bm, i)   ((bm)[(i) >> 2] &= ~(3U << (((i) & 0x3U) << 1))) 

/**
 * return total bytes of the bitmap
 * *n* is the number of elements,
 * as one element needs two bits, one single byte can hold 4 elements
 * note that *n* is power of 2
 */
#define __bitmap_size(n)       ((n) < 4 ? 1 : (n) >> 2)

/**
 * roundup to power of two, 32-bit unsigned int version
 */
#define roundup(x)  ({    \
        --(x);    \
        (x) |= (x) >> 1;    \
        (x) |= (x) >> 2;    \
        (x) |= (x) >> 4;    \
        (x) |= (x) >> 8;    \
        (x) |= (x) >> 16;    \
        ++(x);})

static const double HASH_UPPER = 0.77;
typedef uint32_t hash_iter_t;

#define HASH_DECLARE(name, key_t, val_t)    \
    struct hash_##name {    \
        uint32_t n_buckets;    \
        uint32_t size;    \
        uint32_t n_occupied;    \
        uint32_t upper_bound;    \
        uint8_t *flags;    \
        key_t *keys;    \
        val_t *vals;    \
    }

/**
 * int (*key_equal)(key_t const key1, key_t const key2);
 * return non-zero if equal, or zero if not equal 
 */
#define HASH_IMPL(name, key_t, val_t, hash_func, key_equal)    \
    static inline struct hash_##name *hash_init_##name()    \
    {   \
        struct hash_##name *h = malloc(sizeof(struct hash_##name));    \
        if(!h)    \
            return NULL;    \
        h->size = h->n_occupied = 0;    \
        h->n_buckets = 4;    \
        h->flags = malloc(__bitmap_size(h->n_buckets));    \
        if(!h->flags)    \
            return NULL;    \
        h->flags[0] = 0xAA;    \
        h->upper_bound = (uint32_t) (4 * HASH_UPPER + 0.5);    \
        h->keys = malloc(4 * sizeof(key_t));    \
        if(!h->keys)    \
            return NULL;    \
        h->vals = malloc(4 * sizeof(val_t));    \
        if(!h->vals)    \
            return NULL;    \
        return h;    \
    }   \
    static inline void hash_destroy_##name(struct hash_##name *h)    \
    {   \
        free(h->flags);    \
        free(h->keys);    \
        free(h->vals);    \
        free(h);    \
    }   \
    static inline void hash_clear_##name(struct hash_##name *h)    \
    {   \
        memset(h->flags, 0xAA, __bitmap_size(h->n_buckets));    \
        h->size = h->n_occupied = 0;    \
    }   \
    static inline hash_iter_t hash_find_##name(struct hash_##name *h, key_t key)    \
    {   \
        hash_iter_t i, last;    \
        uint32_t hash, mask, step = 0;    \
        mask = h->n_buckets - 1;    \
        hash = hash_func(key);    \
        last = i = hash & mask;    \
        while(!__is_empty(h->flags, i) && (__is_delete(h->flags, i) || !key_equal(h->keys[i], key))) {    \
            i = (i + (++step)) & mask;    \
            if(i == last)   \
                return h->n_buckets;    \
        }   \
        return __is_empty(h->flags, i) ? h->n_buckets : i;    \
    }   \
    static inline int hash_rehash_##name(struct hash_##name *h, uint32_t new_n_buckets)    \
    {   \
        uint8_t *new_flags = NULL;    \
        uint8_t rehash = 1;    \
        new_n_buckets = roundup(new_n_buckets);    \
        if(h->size >= (uint32_t) (new_n_buckets * HASH_UPPER + 0.5))    \
            rehash = 0;    \
        else {    \
            new_flags = malloc(__bitmap_size(new_n_buckets));    \
            if(!new_flags)    \
                return -1;    \
            memset(new_flags, 0xAA, __bitmap_size(new_n_buckets));    \
            if(h->n_buckets < new_n_buckets) {    \
                key_t *new_keys = realloc(h->keys, new_n_buckets * sizeof(key_t));    \
                if(!new_keys)    \
                    return -1;    \
                val_t *new_vals = realloc(h->vals, new_n_buckets * sizeof(val_t));    \
                if(!new_vals)    \
                    return -1;    \
                h->keys = new_keys;    \
                h->vals = new_vals;    \
            }    \
        }    \
        if(rehash) {    \
            uint32_t new_mask = new_n_buckets - 1;    \
            for(uint32_t j = 0; j < h->n_buckets; j++) {    \
                if(__is_valid(h->flags, j)) {    \
                    key_t key = h->keys[j];    \
                    val_t val = h->vals[j];    \
                    __set_delete(h->flags, j);    \
                    while(1) {    \
                        uint32_t i, step = 0;    \
                        i = hash_func(key) & new_mask;    \
                        while(!__is_empty(new_flags, i))    \
                            i = (i + (++step)) & new_mask;    \
                        __clear_empty(new_flags, i);    \
                        if(i < h->n_buckets && __is_valid(h->flags, i)) {    \
                            key_t ktemp = h->keys[i];    \
                            h->keys[i] = key;    \
                            key = ktemp;    \
                            val_t vtemp = h->vals[i];    \
                            h->vals[i] = val;    \
                            val = vtemp;    \
                            __set_delete(h->flags, i);    \
                        }    \
                        else {    \
                            h->keys[i] = key;    \
                            h->vals[i] = val;    \
                            break;    \
                        }    \
                    }    \
                }    \
            }    \
            if(h->n_buckets > new_n_buckets) {    \
                h->keys = realloc(h->keys, new_n_buckets * sizeof(key_t));    \
                h->vals = realloc(h->vals, new_n_buckets * sizeof(val_t));    \
            }    \
            free(h->flags);    \
            h->flags = new_flags;    \
            h->n_buckets = new_n_buckets;    \
            h->n_occupied = h->size;    \
            h->upper_bound = (uint32_t) (h->n_buckets * HASH_UPPER + 0.5);    \
        }    \
        return 0;    \
    }   \
    static inline int hash_insert_##name(struct hash_##name *h, key_t key, val_t val)    \
    {   \
        if(h->n_occupied >= h->upper_bound) {    \
            if(h->n_buckets > (h->size << 1)) {    \
                if(hash_rehash_##name(h, h->n_buckets - 1) < 0)    \
                    return -2;    \
            }    \
            else if(hash_rehash_##name(h, h->n_buckets + 1) < 0)    \
                return -2;    \
        }    \
        uint32_t step = 0, mask = h->n_buckets - 1;    \
        hash_iter_t i, last;    \
        i = hash_func(key) & mask;    \
        last = i;    \
        while(__is_valid(h->flags, i)) {    \
            if(key_equal(h->keys[i], key))    \
                return -1;    \
            i = (i + (++step)) & mask;    \
            if(i == last)    \
                return -3;    \
        }    \
        if(__is_empty(h->flags, i)) {    \
            h->n_occupied++;    \
        }    \
        h->keys[i] = key;    \
        h->vals[i] = val;    \
        __clear_both(h->flags, i);    \
        h->size++;    \
        return 0;    \
    }   \
    static inline void hash_erase_##name(struct hash_##name *h, hash_iter_t i)    \
    {   \
        __set_delete(h->flags, i);    \
        --h->size;    \
    }    \
    static inline key_t hash_get_key_##name(struct hash_##name *h, hash_iter_t i)    \
    {    \
        return (key_t) h->keys[i];    \
    }    \
    static inline val_t hash_get_val_##name(struct hash_##name *h, hash_iter_t i)    \
    {    \
        return (val_t) h->vals[i];    \
    }

/**
 * public API specification
 */
#define hash_t(name)                 struct hash_##name
#define hash_init(name)              hash_init_##name()
#define hash_destroy(name, h)        hash_destroy_##name(h)
#define hash_clear(name, h)          hash_clear_##name(h)
#define hash_find(name, h, k)        hash_find_##name(h, k)
#define hash_insert(name, h, k, v)   hash_insert_##name(h, k, v)
#define hash_erase(name, h, i)       hash_erase_##name(h, i)
#define hash_get_key(name, h, i)     hash_get_key_##name(h, i)
#define hash_get_val(name, h, i)     hash_get_val_##name(h, i)
#define hash_size(name, h)           (h->size)
#define hash_empty(name, h)          (h->size == 0)
#define hash_valid(name, h, i)       (i < h->n_buckets && __is_valid(h->flags, i))
/* end */


/**
 * signed or unsigned 32-bit integer hash function
 */
#define default_int32_hash_func(key)    ((uint32_t) key)
#define default_int32_hash_equal(a, b)  ((a) == (b))
#define alter_int32_hash_func(key)    ({    \
        uint32_t _k = (uint32_t) key;    \
        _k += ~(_k << 15);    \
        _k ^=  (_k >> 10);    \
        _k +=  (_k << 3);    \
        _k ^=  (_k >> 6);    \
        _k += ~(_k << 11);    \
        _k ^=  (_k >> 16);    \
        _k;})

/**
 * signed or unsigned 64-bit integer hash function
 */
#define default_int64_hash_func(key)    ((uint32_t) ((key) >> 33 ^ (key) ^ (key) << 11))
#define default_int64_hash_equal(a, b)  ((a) == (b))

/**
 * DJB2 string hash function
 */
#define default_string_hash_func(s)     ({    \
        char *_s = (char *) s;    \
        uint32_t _h = 5381;    \
        int _c;    \
        while((_c = *_s++))    \
            _h = ((_h << 5) + _h) + _c;    \
        _h;})
#define default_string_hash_equal(a, b)   (strcmp(a, b) == 0)

#endif // _LIBTINY_HASH_H_
