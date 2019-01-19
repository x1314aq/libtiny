//
// Created by x1314aq on 2018/8/27.
//

#ifndef _LIBTINY_COMMON_H_
#define _LIBTINY_COMMON_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "limits.h"


#ifndef offsetof
#define offsetof(type, member)  ((size_t) &((type *)0)->member)
#endif

#ifndef container_of
#define container_of(ptr, type, member) ({    \
        const typeof(((type *)0)->member)* __mptr = (ptr);    \
        (type *)((char *)__mptr - offsetof(type, member));})
#endif

#ifndef NULL
#define NULL ((void *) 0)
#endif

#ifndef likely
#define likely(x)  __builtin_expect(!!(x),1)
#endif

#ifndef unlikely
#define unlikely(x)  __builtin_expect(!!(x),0)
#endif

#ifndef MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif

#ifndef MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif

#ifndef __CACHE_ALIGNED
#define __CACHE_LINE_SIZE  64
#define __CACHE_LINE_MASK  63
#define __CACHE_ALIGNED __attribute__((__aligned__(__CACHE_LINE_SIZE)))
#endif

/**
 * Function which rounds an unsigned int down to a given power-of-two value.
 * Takes uintptr_t types as parameters, as this type of operation is most
 * commonly done for pointer alignment. (See also RTE_ALIGN_FLOOR,
 * RTE_ALIGN_CEIL, RTE_ALIGN, RTE_PTR_ALIGN_FLOOR, RTE_PTR_ALIGN_CEL,
 * RTE_PTR_ALIGN macros)
 * @param ptr
 *   The value to be rounded down
 * @param align
 *   The power-of-two of which the result must be a multiple.
 * @return
 *   Function returns a properly aligned value where align is a power-of-two.
 *   If align is not a power-of-two, result will be incorrect.
 */
static inline uintptr_t
rte_align_floor_int(uintptr_t ptr, uintptr_t align)
{
    return (ptr & ~(align - 1));
}

/**
 * Macro to align a pointer to a given power-of-two. The resultant
 * pointer will be a pointer of the same type as the first parameter, and
 * point to an address no higher than the first parameter. Second parameter
 * must be a power-of-two value.
 */
#define RTE_PTR_ALIGN_FLOOR(ptr, align) \
	(typeof(ptr))rte_align_floor_int((uintptr_t)ptr, align)

/**
 * Macro to align a value to a given power-of-two. The resultant value
 * will be of the same type as the first parameter, and will be no
 * bigger than the first parameter. Second parameter must be a
 * power-of-two value.
 */
#define RTE_ALIGN_FLOOR(val, align) \
	(typeof(val))((val) & (~((typeof(val))((align) - 1))))

/**
 * Macro to align a pointer to a given power-of-two. The resultant
 * pointer will be a pointer of the same type as the first parameter, and
 * point to an address no lower than the first parameter. Second parameter
 * must be a power-of-two value.
 */
#define RTE_PTR_ALIGN_CEIL(ptr, align) \
	RTE_PTR_ALIGN_FLOOR((typeof(ptr))RTE_PTR_ADD(ptr, (align) - 1), align)

/**
 * Macro to align a value to a given power-of-two. The resultant value
 * will be of the same type as the first parameter, and will be no lower
 * than the first parameter. Second parameter must be a power-of-two
 * value.
 */
#define RTE_ALIGN_CEIL(val, align) \
	RTE_ALIGN_FLOOR(((val) + ((typeof(val)) (align) - 1)), align)

/**
 * Macro to align a pointer to a given power-of-two. The resultant
 * pointer will be a pointer of the same type as the first parameter, and
 * point to an address no lower than the first parameter. Second parameter
 * must be a power-of-two value.
 * This function is the same as RTE_PTR_ALIGN_CEIL
 */
#define RTE_PTR_ALIGN(ptr, align) RTE_PTR_ALIGN_CEIL(ptr, align)

/**
 * Macro to align a value to a given power-of-two. The resultant
 * value will be of the same type as the first parameter, and
 * will be no lower than the first parameter. Second parameter
 * must be a power-of-two value.
 * This function is the same as RTE_ALIGN_CEIL
 */
#define RTE_ALIGN(val, align) RTE_ALIGN_CEIL(val, align)


#if __MAX_LCORE == 1
#define MPLOCKED                        /**< No need to insert MP lock prefix. */
#else
#define MPLOCKED        "lock ; "       /**< Insert MP lock prefix. */
#endif

#define __COMPILER_BARRIER() do {  \
    __asm__ volatile ("" : : : "memory");  \
} while(0)

#ifdef __cplusplus
#define BEGIN_DECL  extern "C" {
#define END_DECL  }
#else
#define BEGIN_DECL
#define END_DECL
#endif

BEGIN_DECL
static inline uint64_t
stl_rdtsc(void)
{
    union {
        uint64_t tsc_64;
        struct _t{
            uint32_t lo_32;
            uint32_t hi_32;
        } t;
    } tsc;

    __asm__ volatile("rdtsc" :
    "=a" (tsc.t.lo_32),
    "=d" (tsc.t.hi_32));
    return tsc.tsc_64;
}

static inline int
stl_atomic32_cmpset(volatile uint32_t *dst, uint32_t exp, uint32_t src)
{
    uint8_t res;

    __asm__ volatile(
    MPLOCKED
    "cmpxchgl %[src], %[dst];"
    "sete %[res];"
    : [res] "=a" (res),     /* output */
    [dst] "=m" (*dst)
    : [src] "r" (src),      /* input */
        "a" (exp),
        "m" (*dst)
    : "memory");            /* no-clobber list */
    return res;
}

END_DECL

#endif // _LIBTINY_COMMON_H_
