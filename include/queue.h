//
//  Created by x1314aq on 2017/7/22.
//

#ifndef _LIBTINY_QUEUE_H_
#define _LIBTINY_QUEUE_H_

#include "common.h"

typedef struct{
    int _max;
    int _size;
    void **_cont;
    int _btm;
    int _top;
} queue;

BEGIN_DECL

#define queue_empty(q) (!(q)->_size)

queue *queue_init(int maxsz);

void queue_push(queue *q, void *ele);

void queue_pop(queue *q);

void *queue_front(queue *q);

void queue_free(queue *q);

END_DECL

#endif // _LIBTINY_QUEUE_H_
