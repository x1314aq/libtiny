//
//  Created by x1314aq on 2017/7/24.
//

#ifndef _LIBTINY_HEAP_H_
#define _LIBTINY_HEAP_H_

#include "common.h"


/* 大顶堆 */
typedef struct{
    int _size;
    int _max;
    void *_cont;
    int (*_cmp)(const void *, const void *);
    size_t _ele_size;
} heap;

BEGIN_DECL

heap *heap_init(int maxsz, size_t ele_size, int (*cmp)(const void *, const void *));

heap *heap_heapify(void *base, int size, size_t ele_size, int (*cmp)(const void *, const void *));

void heap_free(heap *hp);

void heap_insert(heap *hp, void *ele);

void *heap_get_max(heap *hp);

void heap_rm_max(heap *hp);

END_DECL

#endif // _LIBTINY_HEAP_H_
