//
//  queue.h
//  my_stl
//
//  Created by 叶鑫 on 2017/7/22.
//  Copyright © 2017年 xaq. All rights reserved.
//

#ifndef queue_h
#define queue_h

#include <stdlib.h>

typedef struct{
    int _max;
    int _size;
    void **_cont;
    int _btm;
    int _top;
} queue;

#define queue_empty(q) (!(q)->_size)

queue *queue_init(int maxsz);

void queue_push(queue *q, void *ele);

void queue_pop(queue *q);

void *queue_front(queue *q);

void queue_free(queue *q);

#endif /* queue_h */
