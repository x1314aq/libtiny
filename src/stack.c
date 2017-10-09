//
//  stack.c
//  my_stl
//
//  Created by 叶鑫 on 2017/7/22.
//  Copyright © 2017年 xaq. All rights reserved.
//

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


stack *stack_init(int maxsz, size_t ele_size){
    stack *s = malloc(sizeof(stack));
    s->_max = maxsz;
    s->_size = 0;
    s->_ele_size = ele_size;
    s->_cont = malloc(maxsz * ele_size);
    return s;
}

void stack_push(stack *s, void *ele){
    if(s->_size < s->_max){
        MEMCPY(ele, s->_cont + s->_ele_size * s->_size, s->_ele_size);
        s->_size++;
    }
}

void stack_pop(stack *s){
    if(s->_size > 0) s->_size--;
}

void *stack_top(stack *s){
    if(s->_size > 0) return s->_cont + (s->_size - 1) * s->_ele_size;
    return NULL;
}

void stack_free(stack *s){
    free(s->_cont);
    s->_cont = NULL;
    free(s);
}
