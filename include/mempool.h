//
// Created by x1314aq on 2019/1/31.
//

#ifndef _LIBTINY_MEMPOOL_H_
#define _LIBTINY_MEMPOOL_H_

#include "common.h"


#define ALIGN          8
#define ALIGN_MASK     (ALIGN - 1)
#define NUM_FREE_LIST  16
#define MAX_ALLOC      32

union obj {
    union obj *next;
    char data[1];
};

struct mempool {
    union obj *free_lists[NUM_FREE_LIST];
    char *start;
    char *end;
    size_t heap_size;
    void *addr[MAX_ALLOC];
    uint8_t index;
};

BEGIN_DECL

void
mempool_init(struct mempool *mp);

void
mempool_destroy(struct mempool *mp);

void *
mempool_alloc(struct mempool *mp, size_t n);

void
mempool_dealloc(struct mempool *mp, void *p, size_t n);

END_DECL

#endif // _LIBTINY_MEMPOOL_H_
