//
//  stack.h
//  my_stl
//
//  Created by 叶鑫 on 2017/7/22.
//  Copyright © 2017年 xaq. All rights reserved.
//

#ifndef stack_h
#define stack_h

#include <stdlib.h>

typedef struct{
    int _max;
    int _size;
    size_t _ele_size;
    void *_cont;
} stack;

#define stack_empty(s) (!(s)->_size)

stack *stack_init(int maxsz, size_t ele_size);

void stack_push(stack *s, void *ele);

void stack_pop(stack *s);

void *stack_top(stack *s);

void stack_free(stack *s);

#endif /* stack_h */
