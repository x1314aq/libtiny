//
// Created by 叶鑫 on 2018/9/22.
//

#ifndef STL_MEMPOOL_H
#define STL_MEMPOOL_H

#include "common.h"


#define ALIGN          8
#define ALIGN_MASK    (ALIGN-1)
#define NUM_FREE_LIST  16

union obj {
    union obj *next;
    char data[1];
};

struct mempool {
    union obj *free_lists[NUM_FREE_LIST];
    uint8_t *start;
    uint8_t *end;
    uint8_t *heap_size;
};

static inline size_t
round_up(size_t x)
{
    return (x + ALIGN_MASK) & ~ALIGN_MASK;
}

static inline size_t
free_list_index(size_t x)
{
    return (x + ALIGN_MASK) / ALIGN - 1;
}

void *
mempool_alloc(struct mempool *mp,
              size_t n);

void
mempool_dealloc(struct mempool *mp,
                void *p,
                size_t n);

#endif //STL_MEMPOOL_H
