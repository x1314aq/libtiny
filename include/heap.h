//
//  Created by x1314aq on 2019/03/08.
//

#ifndef _LIBTINY_HEAP_H_
#define _LIBTINY_HEAP_H_

#include "common.h"

#define __lc(i)    (2 * (i) + 1)
#define __rc(i)    (2 * ((i) + 1))
#define __pa(i)    (((i) - 1) / 2)

#define HEAP_DECLARE(name, type)    \
    struct heap_##name {    \
        int num;    \
        int max;    \
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
        int index = h->num++;    \
        int next = __pa(index);    \
        while(index > 0 && lt_func(h->room[next], ele)) {    \
            h->room[index] = h->room[next];    \
            index = next;    \
            next = __pa(index);    \
        }    \
        h->room[index] = ele;    \
    }    \
    static inline void heap_percolate_down_##name(struct heap_##name *h, type ele)    \
    {    \
        int index = 0;    \
        int next = 2;    \
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
        int new_max = h->max + (h->max >> 1);    \
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
    }    \
    static inline void heap_make_##name(type *start, int num)    \
    {    \
        if(num < 2)    \
            return;    \
        int index = (num - 2) / 2;    \
        type ele = start[index];    \
        while(1) {    \
            int index2 = index;    \
            int next = index2 * 2 + 2;    \
            while(next < num) {    \
                if(lt_func(start[next], start[next - 1]))    \
                    next--;    \
                start[index2] = start[next];    \
                index2 = next;    \
                next = __rc(index2);    \
            }    \
            if(next == num) {    \
                start[index2] = start[next - 1];    \
                index2 = next - 1;    \
            }    \
            next = __pa(index2);    \
            while(index2 > index && lt_func(start[next], ele)) {    \
                start[index2] = start[next];    \
                index2 = next;    \
                next = __pa(index2);    \
            }    \
            start[index2] = ele;    \
            if(index == 0)    \
                return;    \
            index--;    \
            ele = start[index];    \
        }    \
    }    \
    static inline void heap_sort_##name(type *start, int num)    \
    {    \
        struct heap_##name h = { .num = num, .max = num, .room = start };    \
        heap_make_##name(start, num);    \
        while(h.num > 1) {    \
            type temp = h.room[0];    \
            heap_percolate_down_##name(&h, h.room[--h.num]);    \
            h.room[h.num] = temp;    \
        }    \
    }

/**
 * public API specification
 */
#define heap_t(name)             struct heap_##name
#define heap_init(name)          heap_init_##name()
#define heap_destroy(name, h)    heap_destroy_##name(h)
#define heap_push(name, h, ele)  heap_push_##name(h, ele)
#define heap_pop(name, h)        heap_pop_##name(h)
#define heap_make(name, s, n)    heap_make_##name(s, n)
#define heap_sort(name, s, n)    heap_sort_##name(s, n)

#endif // _LIBTINY_HEAP_H_
