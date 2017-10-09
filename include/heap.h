//
//  heap.h
//  my_stl
//
//  Created by 叶鑫 on 2017/7/24.
//  Copyright © 2017年 xaq. All rights reserved.
//

#ifndef heap_h
#define heap_h

#include <stdlib.h>


/* 大顶堆 */
typedef struct{
    int _size;
    int _max;
    void *_cont;
    int (*_cmp)(const void *, const void *);
    size_t _ele_size;
} heap;

heap *heap_init(int maxsz, size_t ele_size, int (*cmp)(const void *, const void *));

heap *heap_heapify(void *base, int size, size_t ele_size, int (*cmp)(const void *, const void *));

void heap_free(heap *hp);

void heap_insert(heap *hp, void *ele);

void *heap_get_max(heap *hp);

void heap_rm_max(heap *hp);

#endif /* heap_h */
