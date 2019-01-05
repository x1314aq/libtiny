//
// Created by 叶鑫 on 2018/9/8.
//

#include "ringbuf.h"


static inline int
__is_power_of_2(unsigned x)
{
    return (x & (x - 1)) == 0;
}

static inline ssize_t
__get_memsize(unsigned size)
{
    ssize_t ret;

    if(!__is_power_of_2(size))
        return -EINVAL;

    ret = sizeof(struct ringbuf) + size * sizeof(void *);
    ret = RTE_ALIGN(ret, __CACHE_LINE_SIZE);
    return ret;
}

int
ringbuf_init(struct ringbuf *rb,
             const char *name,
             unsigned size)
{
    memset(rb, 0, sizeof(*rb));
    snprintf(rb->name, RINGBUF_NAMESIZE, "%s", name);
    rb->size = size;
    rb->mask = size - 1;
    rb->prod.head = rb->cons.head = 0;
    rb->prod.tail = rb->cons.tail = 0;
    return 0;
}

struct ringbuf *
ringbuf_create(const char *name,
               unsigned size)
{
    struct ringbuf *rb;
    ssize_t rb_size;

    if(rb_size = __get_memsize(size)) {
        errno = -EINVAL;
        return NULL;
    }

    return rb;
}
