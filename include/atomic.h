//
// Created by x1314aq on 2019/12/1
//

/**
 * atomic operations implemented by compiler built-in functions
 */

#ifndef _ATOMIC_H_
#define _ATOMIC_H_

typedef struct {
    int counter;
} atomic_t;

#ifdef __cplusplus
extern "C" {
#endif

static inline int atomic_read(atomic_t *v)
{
    return __atomic_load_n(&v->counter, __ATOMIC_SEQ_CST);
}

static inline void atomic_set(atomic_t *v, int n)
{
    __atomic_store_n(&v->counter, n, __ATOMIC_SEQ_CST);
}

static inline void atomic_inc(atomic_t *v)
{
    __atomic_add_fetch(&v->counter, 1, __ATOMIC_SEQ_CST);
}

static inline void atomic_dec(atomic_t *v)
{
    __atomic_sub_fetch(&v->counter, 1, __ATOMIC_SEQ_CST);
}

static inline int atomic_dec_and_test(atomic_t *v)
{
    return __atomic_sub_fetch(&v->counter, 1, __ATOMIC_SEQ_CST) == 0;
}

#ifdef __cplusplus
}
#endif

#endif
