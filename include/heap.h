//
//  Created by x1314aq on 2019/03/08.
//

#ifndef _LIBTINY_HEAP_H_
#define _LIBTINY_HEAP_H_

#include "common.h"

#define __lc(i)    (2 * (i) + 1)
#define __rc(i)    (2 * ((i) + 1))
#define __pa(i)    (((i) - 1) << 1)

#define HEAP_DECLARE(name, type)    \
    struct heap_##name {    \
        size_t num;    \
        size_t max;    \
        type *room;    \
    }

#define HEAP_IMPL(name, type, lt_func)    \
    static inline struct heap_##name *heap_init_##name()    \
    {    \
        struct heap_##name *h = malloc(sizeof(struct heap_##name));    \
        if(!h)    \
            return NULL;    \
        h->num = 0;    \
        h->max = 4;    \
        h->room = malloc(h->max * sizeof(type));    \
        if(!h->room) {    \
            free(h);    \
            return NULL;    \
        }    \
        return h;    \
    }    \
    static inline void heap_destroy_##name(struct heap_##name *h)    \
    {    \
        free(h->room);    \
        free(h);    \
    }    \
    static inline void heap_percolate_up_##name(struct heap_##name *h, type ele)    \
    {    \
        size_t index = h->num++;    \
        size_t next = __pa(index);    \
        while(index > 0 && lt_func(h->room[next], ele)) {    \
            h->room[index] = h->room[next];    \
            index = next;    \
            next = __pa(index);    \
        }    \
        h->room[index] = ele;    \
    }    \
    static inline void heap_percolate_down_##name(struct heap_##name *h, type ele)    \
    {    \
        size_t index = 0;    \
        size_t next = 2;    \
        while(next < h->num) {    \
            if(lt_func(h->room[next], h->room[next - 1]))    \
                next--;    \
            h->room[index] = h->room[next];    \
            index = next;    \
            next = __rc(index);    \
        }    \
        if(next == h->num) {    \
            h->room[index] = h->room[next - 1];    \
            index = next - 1;    \
        }    \
        next = __pa(index);    \
        while(index > 0 && lt_func(h->room[next], ele)) {    \
            h->room[index] = h->room[next];    \
            index = next;    \
            next = __pa(index);    \
        }    \
        h->room[index] = ele;    \
    }    \
    static inline int heap_resize_##name(struct heap_##name *h)    \
    {    \
        size_t new_max = h->max + h->max >> 1;    \
        void *temp = realloc(h->room, sizeof(type) * new_max);    \
        if(!temp)    \
            return -1;    \
        h->max = new_max;    \
        h->room = temp;    \
        return 0;    \
    }    \
    static inline int heap_push_##name(struct heap_##name *h, type ele)    \
    {    \
        if(h->num == h->max) {    \
            if(heap_resize_##name(h))    \
                return -1;    \
        }    \
        heap_percolate_up_##name(h, ele);    \
        return 0;    \
    }    \
    static inline type heap_pop_##name(struct heap_##name *h)    \
    {    \
        type temp = h->room[0];    \
        heap_percolate_down_##name(h, h->room[--h->num]);    \
        return temp;    \
    }

/**
 * public API specification
 */
#define heap_t(name)             struct heap_##name
#define heap_init(name)          heap_init_##name()
#define heap_destroy(name, h)    heap_destroy_##name(h)
#define heap_push(name, h, ele)  heap_push_##name(h, ele)
#define heap_pop(name, h)        heap_pop_##name(h)

#endif // _LIBTINY_HEAP_H_
