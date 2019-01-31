//
// Created by x1314aq on 2019/1/31.
//

#include "mempool.h"


#define MEM_THRESHOLD    NUM_FREE_LIST * ALIGN
#define NUM_OBJS         32

/**
 * Round up to multiple of ALIGN
 */
static inline size_t
__round_up(size_t x)
{
    return (x + ALIGN_MASK) & ~ALIGN_MASK;
}

static inline size_t
__index(size_t x)
{
    return (x + ALIGN_MASK) / ALIGN - 1;
}

static char *
__chunk_alloc(struct mempool *mp, size_t n, int *m)
{
    char *result;
    size_t total_bytes = n * (*m);
    size_t bytes_left = mp->end - mp->start;

    if(bytes_left > total_bytes) {
        result = mp->start;
        mp->start += total_bytes;
    }
    else if(bytes_left >= n) {
        *m = bytes_left / n;
        total_bytes = n * (*m);
        result = mp->start;
        mp->start += total_bytes;
    }
    else {
        size_t bytes_to_get = 2 * total_bytes + __round_up(mp->heap_size);
        if(bytes_left > 0) {
            union obj **free_list = mp->free_lists + __index(bytes_left);
            ((union obj *) mp->start)->next = *free_list;
            *free_list = (union obj *) mp->start;
        }
        mp->start = (char *) malloc(bytes_to_get);
        if(!mp->start) {
            *m = 0;
            return NULL;
        }
        mp->heap_size += bytes_to_get;
        mp->end = mp->start + bytes_to_get;
        result = mp->start;
        mp->start += total_bytes;
    }
    return result;
}

static void *
__refill(struct mempool *mp, size_t n)
{
    int nobjs = NUM_OBJS;
    char *chunk;
    char **free_list;
    union obj *cur, *next;

    chunk = __chunk_alloc(mp, n, &nobjs);

    if(!chunk || nobjs == 1)
        return chunk;

    free_list = mp->free_lists + __index(n);
    *free_list = next = (union obj *) (chunk + n);
    for(int i = 1; i < nobjs; i++) {
        cur = next;
        next = (union obj *) ((char *) cur + n);
        cur->next = next;
    }
    cur->next = NULL;
    return chunk;
}

void
mempool_init(struct mempool *mp)
{
    mp->start = NULL;
    mp->end = NULL;
    mp->heap_size = 0;
    for(int i = 0; i < NUM_FREE_LIST; i++)
        mp->free_lists[i] = NULL;
}

void
mempool_destroy(struct mempool *mp)
{
    return;
}

void *
mempool_alloc(struct mempool *mp, size_t n)
{
    union obj *result;
    union obj **free_list;

    if(n > (size_t) MEM_THRESHOLD)
        return malloc(n);

    free_list = mp->free_lists + __index(n);
    result = *free_list;

    if(!result)
        return __refill(mp, __round_up(n));

    *free_list = result->next;
    return result;
}

void
mempool_dealloc(struct mempool *mp, void *p, size_t n)
{
    union obj *q = (union obj *) p;
    union obj **free_list;

    if(n > (size_t) MEM_THRESHOLD) {
        free(p);
        return;
    }

    free_list = mp->free_lists + __index(n);
    q->next = *free_list;
    *free_list = q;
}
