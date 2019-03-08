//
// Created by x1314aq on 2019/03/06.
//

#ifndef _LIBTINY_VECTOR_H_
#define _LIBTINY_VECTOR_H_

#include "common.h"

#define VECTOR_DECLARE(name, type)    \
    struct vec_##name {    \
        size_t num;    \
        size_t max;    \
        type *room;    \
    }

#define VECTOR_IMPL(name, type)    \
    static inline struct vec_##name *vec_init_##name()    \
    {    \
        struct vec_##name *v = malloc(sizeof(struct vec_##name));    \
        if(!v)    \
            return NULL;    \
        v->num = 0;    \
        v->max = 4;    \
        v->room = malloc(v->max * sizeof(type));    \
        if(!v->room) {    \
            free(v);    \
            return NULL;    \
        }    \
        return v;    \
    }    \
    static inline void vec_destroy_##name(struct vec_##name *v)    \
    {    \
        free(v->room);    \
        free(v);    \
    }    \
    static inline int vec_resize_##name(struct vec_##name *v)    \
    {    \
        size_t new_max = v->max + v->max >> 1;    \
        void *temp = realloc(v->room, sizeof(type) * new_max);    \
        if(!temp)    \
            return -1;    \
        v->max = new_max;    \
        v->room = temp;    \
        return 0;    \
    }    \
    static inline int vec_push_##name(struct vec_##name *v, type ele)    \
    {    \
        if(v->num == v->max) {    \
            if(vec_resize_##name(v))    \
                return -1;    \
        }    \
        v->room[v->num++] = ele;    \
        return 0;    \
    }    \
    static inline type vec_pop_##name(struct vec_##name *v)    \
    {    \
        return v->room[--v->num];    \
    }

/**
 * public API sepcification
 */
#define vec_t(name)             struct vec_##name
#define vec_init(name)          vec_init_##name()
#define vec_destroy(name, v)    vec_destroy_##name(v)
#define vec_push(name, v, e)    vec_push_##name(v, e)
#define vec_pop(name, v)        vec_pop_##name(v)
#define vec_clear(name, v)      ((v)->num = 0)
#define vec_at(name, v, i)      ((v)->room[(i)])
#define vec_size(name, v)       ((v)->num)
#define vec_capacity(name, v)   ((v)->max)
#define vec_empty(name ,v)      (vec_size(name, v) == 0)
/* end */

#endif //_LIBTINY_VECTOR_H_
