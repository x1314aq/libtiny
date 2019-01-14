//
//  Created by x1314aq on 2017/7/22.
//

#ifndef _LIBTINY_STACK_H_
#define _LIBTINY_STACK_H_

#include "common.h"

typedef struct{
    int _max;
    int _size;
    size_t _ele_size;
    void *_cont;
} stack;

#define stack_empty(s) (!(s)->_size)

BEGIN_DECL

stack *stack_init(size_t ele_size);

void stack_push(stack *s, void *ele);

void stack_pop(stack *s);

void *stack_top(stack *s);

void stack_free(stack *s);

int stack_size(stack *s);

END_DECL

#endif // _LIBTINY_STACK_H_
