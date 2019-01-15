//
// Created by x1314aq on 2018/9/22.
//

#include "mempool.h"


void *
__mempool_refill(struct mempool *mp,
                 size_t n)
{

}

void *
mempool_alloc(struct mempool *mp,
              size_t n)
{
    union obj *result;
    union obj **my_free_list;

    size_t sz = round_up(n);
    my_free_list = mp->free_lists + free_list_index(sz);

    result = *my_free_list;

    if(result == NULL) {
        return __mempool_refill(mp, sz);
    }

    *my_free_list = result->next;
    return result;
}

void
mempool_dealloc(struct mempool *mp,
                void *p,
                size_t n)
{
    union obj *q = (union obj *) p;
    union obj **my_free_list;
    size_t sz = round_up(n);

    my_free_list = mp->free_lists + free_list_index(sz);
    q->next = *my_free_list;
    *my_free_list = q;
}
