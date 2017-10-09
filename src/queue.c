//
//  queue.c
//  my_stl
//
//  Created by 叶鑫 on 2017/7/22.
//  Copyright © 2017年 xaq. All rights reserved.
//

#include "queue.h"


queue *queue_init(int maxsz){
    queue *q = malloc(sizeof(queue));
    q->_size = 0;
    q->_max = maxsz;
    q->_btm = 0;
    q->_top = 0;
    q->_cont = malloc(maxsz * sizeof(void *));
    return q;
}

void queue_push(queue *q, void *ele){
    if(q->_size < q->_max){
        q->_size++;
        q->_cont[q->_btm] = ele;
        q->_btm = (q->_btm + 1) % q->_max;
    }
}

void queue_pop(queue *q){
    if(q->_size > 0){
        q->_size--;
        q->_top = (q->_top + 1) % q->_max;
    }
}

void *queue_front(queue *q){
    if(q->_size > 0) return q->_cont[q->_top];
    return NULL;
}

void queue_free(queue *q){
    for(int i = 0; i < q->_size; i++){
        free(q->_cont[i]);
    }
    free(q->_cont);
    free(q);
}
