//
// Created by x1314aq on 2019/03/17.
//

/**
 * @file
 * This file provides three sort algorithms: merge_sort(), comb_sort() and intro_sort(),
 * heap_sort() is provided by heap.h.
 */

#include "common.h"


struct intro_sort_stack {
    void *left;
    void *right;
    int depth;
};

#define SORT_IMPL(name, type, lt_func)    \
    static void merge_sort_##name(type *arr, size_t n)    \
    {    \
        type *a2[2], *a, *b;    \
        int cur, shift;    \
        a2[0] = arr;    \
        a2[1] = (type *) malloc(n * sizeof(type));    \
        for(cur = 0, shift = 0; (1UL << shift) < n; shift++) {    \
            a = a2[cur];    \
            b = a2[1 - cur];    \
            if(likely(shift != 0)) {    \
                size_t i, step = 1UL << shift;    \
                for(i = 0; i < n; i += step << 1) {    \
                    type *p, *j, *k, *ea, *eb;    \
                    if(unlikely(n < i + step)) {    \
                        ea = a + n;    \
                        eb = a;    \
                    }    \
                    else {    \
                        ea = a + i + step;    \
                        eb = a + (n < i + (step << 1) ? n : i + (step << 1));    \
                    }    \
                    j = a + i;    \
                    k = a + i + step;    \
                    p = b + i;    \
                    while(j < ea && k < eb) {    \
                        if(lt_func(*k, *j))    \
                            *p++ = *k++;    \
                        else    \
                            *p++ = *j++;    \
                    }    \
                    while(j < ea)    \
                        *p++ = *j++;    \
                    while(k < eb)    \
                        *p++ = *k++;    \
                }    \
            }    \
            else {    \
                type *p = b, *i, *eb = a + n;    \
                for(i = a; i < eb; i += 2) {    \
                    if(likely(i != eb - 1)) {    \
                        if(lt_func(*(i + 1), *i)) {    \
                            *p++ = *(i + 1);    \
                            *p++ = *i;    \
                        }    \
                        else {    \
                            *p++ = *i;    \
                            *p++ = *(i + 1);    \
                        }    \
                    }    \
                    else    \
                        *p++ = *i;    \
                }    \
            }    \
            cur = 1 - cur;    \
        }    \
        if(cur == 1) {    \
            type *p = a2[0], *i = a2[1], *eb = arr + n;    \
            for(; p < eb; i++)    \
                *p++ = *i;    \
        }    \
        free(a2[1]);    \
    }    \
    static inline void insert_sort_##name(type *s, type *t)    \
    {    \
        type *i, *j, temp;    \
        for(i = s + 1; i < t; i++) {    \
            for(j = i; j > s && lt_func(*j, *(j - 1)); --j) {    \
                temp = *j;    \
                *j = *(j - 1);    \
                *(j - 1) = temp;    \
            }    \
        }    \
    }    \
    static void comb_sort_##name(type *arr, size_t n)    \
    {    \
        const double shrink_factor = 0.801711847137793;    \
        size_t gap = n;    \
        int swap;    \
        type temp, *i, *j;    \
        do {    \
            if(gap > 2) {    \
                gap = (size_t) (gap * shrink_factor);    \
                if(gap == 9 || gap == 10)    \
                    gap = 11;    \
            }    \
            swap = 0;    \
            for(i = arr; i < arr + n - gap; i++) {    \
                j = i + gap;    \
                if(lt_func(*j, *i)) {    \
                    temp = *i;    \
                    *i = *j;    \
                    *j = temp;    \
                    swap = 1;    \
                }    \
            }    \
        } while(swap || gap > 2);    \
        if(gap != 1)    \
            insert_sort_##name(arr, arr + n);    \
    }    \
    static void intro_sort_##name(type *arr, size_t n)    \
    {    \
        int d;    \
        struct intro_sort_stack *top, *stack;    \
        type rp, temp;    \
        type *s, *t, *i, *j, *k;    \
        if(n < 10) {    \
            insert_sort_##name(arr, arr + n);    \
            return;    \
        }    \
        for(d = 2; (1UL << d) < n; d++);    \
        stack = (struct intro_sort_stack *) malloc(sizeof(struct intro_sort_stack) * (sizeof(size_t) * d + 2));    \
        top = stack;    \
        s = arr;   \
        t = arr + (n - 1);    \
        d <<= 1;    \
        while(1) {    \
            if(s < t) {    \
                if(--d == 0) {    \
                    comb_sort_##name(s, t - s + 1);    \
                    t = s;    \
                    continue;    \
                }    \
                i = s;    \
                j = t;    \
                k = i + ((j - i) >> 1) + 1;    \
                if(lt_func(*k, *i)) {    \
                    if(lt_func(*k, *j))    \
                        k = j;    \
                }    \
                else    \
                    k = lt_func(*j, *i) ? i : j;    \
                rp = *k;    \
                if(k != t) {    \
                    temp = *k;    \
                    *k = *t;    \
                    *t = temp;    \
                }    \
                while(1) {    \
                    do ++i; while(lt_func(*i, rp));    \
                    do --j; while(i <= j && lt_func(rp, *j));    \
                    if(i >= j)    \
                        break;    \
                    temp = *i;    \
                    *i = *j;    \
                    *j = temp;    \
                }    \
                temp = *i;    \
                *i = *j;    \
                *j = temp;    \
                if(i - s > t - i) {    \
                    if(i - s > 16) {    \
                        top->left = s;    \
                        top->right = i - 1;    \
                        top->depth = d;    \
                        ++top;    \
                    }    \
                    s = t - i > 16 ? i + 1 : t;    \
                }    \
                else {    \
                    if(t - i > 16) {    \
                        top->left = i + 1;    \
                        top->right = t;    \
                        top->depth = d;    \
                        top++;    \
                    }    \
                    t = i - s > 16 ? i - 1 : s;    \
                }    \
            }    \
            else {    \
                if(top == stack) {    \
                    free(stack);    \
                    insert_sort_##name(arr, arr + n);    \
                    return;    \
                }    \
                else {    \
                    top--;    \
                    s = (type *) top->left;    \
                    t = (type *) top->right;    \
                    d = top->depth;    \
                }    \
            }    \
        }    \
    }    \


#define merge_sort(name, arr, n)    merge_sort_##name(arr, n)
#define comb_sort(name, arr, n)     comb_sort_##name(arr, n)
#define intro_sort(name, arr, n)    intro_sort_##name(arr, n)
