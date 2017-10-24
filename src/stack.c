//
//  stack.c
//  my_stl
//
//  Created by 叶鑫 on 2017/7/22.
//  Copyright © 2017年 xaq. All rights reserved.
//

#include <stack.h>
#include "stack.h"


#define MEMCPY(src, des, size)           \
    do{                                  \
        size_t __size = (size);          \
        register char *__des = (des);    \
        register char *__src = (src);    \
        do{                              \
            *__des++ = *__src++;         \
        } while(--__size > 0);           \
    } while(0)

#define STACK_INIT_SIZE    4

/* -------------------------- private prototypes ---------------------------- */
void _stack_expand(stack *s);

/* ----------------------------- API implementation ------------------------- */
stack *stack_init(size_t ele_size){
    stack *s = malloc(sizeof(stack));
    s->_max = STACK_INIT_SIZE;
    s->_size = 0;
    s->_ele_size = ele_size;
    s->_cont = malloc(STACK_INIT_SIZE * ele_size);
    return s;
}

void stack_push(stack *s, void *ele){
    if(s->_size == s->_max) _stack_expand(s);
    MEMCPY(ele, s->_cont + s->_ele_size * s->_size, s->_ele_size);
    s->_size++;
}

inline void stack_pop(stack *s){
    if(s->_size > 0) s->_size--;
}

inline void *stack_top(stack *s){
    return s->_size > 0 ? s->_cont + (s->_size - 1) * s->_ele_size : NULL;
}

void stack_free(stack *s){
    free(s->_cont);
    s->_cont = NULL;
    free(s);
}

inline int stack_size(stack *s){
    return s->_size;
}
/* ------------------------ private API implementation ---------------------- */
void _stack_expand(stack *s){
    int new_max = s->_max << 1;
    void *new_cont = malloc(new_max * s->_ele_size);
    MEMCPY(s->_cont, new_cont, s->_ele_size * s->_size);
    free(s->_cont);
    s->_cont = new_cont;
    s->_max = new_max;
}