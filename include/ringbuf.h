//
// Created by x1314aq on 2018/9/1.
//

#ifndef _LIBTINY_RINGBUF_H_
#define _LIBTINY_RINGBUF_H_

#include "common.h"


#define RINGBUF_NAMESIZE    32

enum rb_behavior {
    RB_FIXED = 0,    /*!< enqueue/dequeue a fixed number of items from ringbuf */
    RB_VARIABLE      /*!< enqueue/dequeue as many items as possible from ringbuf */
};

struct rb_headtail {
    volatile uint32_t head;
    volatile uint32_t tail;
};

/**
 * The ringbuf structure.
 *
 * The producer and the consumer have a head and a tail index. The particularity
 * of these index is that they are not between 0 and size(ring). These indexes
 * are between 0 and 2^32, and we mask their value when we access the ring[]
 * field. Thanks to this assumption, we can do subtractions between 2 index
 * values in a modulo-32bit base: that's why the overflow of the indexes is not
 * a problem.
 */
struct ringbuf {
    char name[RINGBUF_NAMESIZE];    /*!< name of the ringbuf */
    uint32_t size;    /*!< maximum size of the ringbuf */
    uint32_t mask;    /*!< mask (size - 1) of the ringbuf */
    struct rb_headtail prod __CACHE_ALIGNED;    /*!< producer structure */
    struct rb_headtail cons __CACHE_ALIGNED;    /*!< consumer structure */
    void *ring[0] __CACHE_ALIGNED;    /*!< memory space starts here */
};


#define ENQUEUE_PTRS()  do {  \
    const uint32_t size = rb->size;  \
    uint32_t index = prod_head & mask;  \
    if(index + n < size) {  \
        for(i = 0; i < (n & (~(unsigned)0x3)); i += 4, index += 4) {  \
            rb->ring[index] = obj_table[i];  \
            rb->ring[index + 1] = obj_table[i + 1];  \
            rb->ring[index + 2] = obj_table[i + 2];  \
            rb->ring[index + 3] = obj_table[i + 3];  \
        }  \
        switch(n & 0x3) {  \
            case 3: rb->ring[index++] = obj_table[i++];  \
            case 2: rb->ring[index++] = obj_table[i++];  \
            case 1: rb->ring[index++] = obj_table[i++];  \
        }  \
    } else {  \
        for(i = 0; index < size; i++, index++)  \
            rb->ring[index] = obj_table[i];  \
        for(index = 0; i < n; i++, index++)  \
            rb->ring[index] = obj_table[i];  \
    }  \
} while(0)

#define DEQUEUE_PTRS()  do {  \
    const uint32_t size = rb->size;  \
    uint32_t index = cons_head & mask;  \
    if(index + n < size) {  \
        for(i = 0; i < (n & (~(unsigned)0x3)); i += 4, index += 4) {  \
            obj_table[i] = rb->ring[index];  \
            obj_table[i + 1] = rb->ring[index + 1];  \
            obj_table[i + 2] = rb->ring[index + 2];  \
            obj_table[i + 3] = rb->ring[index + 3];  \
        }  \
        switch(n & 0x3) {  \
            case 3: obj_table[i++] = rb->ring[index++];  \
            case 2: obj_table[i++] = rb->ring[index++];  \
            case 1: obj_table[i++] = rb->ring[index++];  \
        }  \
    } else {  \
        for(i = 0; index < size; i++, index++)  \
            obj_table[i] = rb->ring[index];  \
        for(index = 0; i < n; i++, index++)  \
            obj_table[i] = rb->ring[index];  \
    }  \
} while(0)

BEGIN_DECL

/**
 * Create a new ringbuf named *name* in memory.
 *
 * The new ring size is set to *size*, which must be a power of two.
 *
 * The ringbuf is added in RTE_TAILQ_RING list.
 *
 * @param name
 *   The name of the ringbuf.
 * @param size
 *   The maximum number of elements in the ringbuf.
 * @return
 *   On success, the pointer to the new allocated ringbuf.
 *   NULL on error with errno set appropriately.
 *   Possible errno values include:
 *    - E_RTE_NO_CONFIG - function could not get pointer to rte_config structure
 *    - E_RTE_SECONDARY - function was called from a secondary process instance
 *    - E_RTE_NO_TAILQ - no tailq list could be got for the ring list
 *    - EINVAL - count provided is not a power of 2
 *    - ENOSPC - the maximum number of memzones has already been allocated
 *    - EEXIST - a memzone with the same name already exists
 *    - ENOMEM - no appropriate memory area found in which to create memzone
 */
struct ringbuf *
ringbuf_create(const char *name,
               unsigned size);

/**
 * Initialize a ringbuf structure.
 *
 * Initialize a ringbuf structure in memory pointed by *rb*. The size
 * of the memory area must be large enough to store the ring structure
 * and the object table. The size is set to *size*, which must be a
 * power of two.
 *
 * The ringbuf is not added in RTE_TAILQ_RING global list. Indeed, the
 * memory given by the caller may not be shareable.
 *
 * @param rb
 *   The pointer to the ringbuf structure.
 * @param name
 *   The name of the ringbuf.
 * @param size
 *   The maximum number of elements in the ringbuf.
 * @return
 *   0 on success, or a negative value on error.
 */
int
ringbuf_init(struct ringbuf *rb,
             const char *name,
             unsigned size);

/**
 * Search a ringbuf from its name
 *
 * @param name
 *   The name of the ringbuf.
 * @return
 *   The pointer to the ringbuf matching the name,
 *   or NULL if not found, with errno set appropriately.
 *   Possible errno values include:
 *    - ENOENT - required entry not available to return.
 */
struct ringbuf *
ringbuf_lookup(const char *name);

/**
 * Enqueue several objects on a ringbuf (NOT multi-producers safe).
 *
 * @param rb
 *   A pointer to the ringbuf structure.
 * @param obj_table
 *   A pointer to a table of void * pointers (objects).
 * @param n
 *   The number of objects to add in the ringbuf from the obj_table.
 * @param behavior
 *   RB_FIXED:    Enqueue a fixed number of items from a ringbuf.
 *   RB_VARIABLE: Enqueue as many items as possible from ringbuf.
 * @return
 *   Depend on the behavior value
 *   if behavior = RB_FIXED
 *   - 0: Success; objects enqueue.
 *   - -ENOBUFS: Not enough room in the ringbuf to enqueue;
 *     no object is enqueued.
 *   if behavior = RB_VARIABLE
 *   - n: Actual number of objects enqueued.
 */
static inline int
ringbuf_sp_enqueue(struct ringbuf *rb,
                   void *const *obj_table,
                   unsigned n,
                   enum rb_behavior behavior)
{
    uint32_t prod_head, cons_tail, prod_next;
    uint32_t free_entries, mask;
    unsigned i;

    prod_head = rb->prod.head;
    cons_tail = rb->cons.tail;
    mask = rb->mask;
    free_entries = mask + cons_tail - prod_head;

    if(free_entries < n) {
        if(behavior == RB_FIXED)
            return -ENOBUFS;
        else {
            if(free_entries == 0)
                return 0;
            n = free_entries;
        }
    }

    prod_next = prod_head + n;
    rb->prod.head = prod_next;
    ENQUEUE_PTRS();
    __COMPILER_BARRIER();
    rb->prod.tail = prod_next;

    return behavior == RB_FIXED ? 0 : n;
}

/**
 * Dequeue several objects from a ringbuf (NOT multi-consumers safe).
 *
 * When the request objects are more than the available objects,
 * only dequeue the actual number of objects.
 *
 * @param rb
 *   A pointer to the ringbuf structure.
 * @param obj_table
 *   A pointer to a table of void * pointers (objects) that will be filled.
 * @param n
 *   The number of objects to dequeue from the ringbuf to the obj_table.
 * @param behavior
 *   RB_FIXED:    Dequeue a fixed number of items from a ringbuf
 *   RB_VARIABLE: Dequeue as many items as possible from ringbuf
 * @return
 *   Depend on the behavior value
 *   if behavior = RB_FIXED
 *   - 0: Success; objects dequeued.
 *   - -ENOENT: Not enough entries in the ringbuf to dequeue;
 *     no object is dequeued.
 *   if behavior = RB_VARIABLE
 *   - n: Actual number of objects dequeued.
 */
static inline int
ringbuf_sc_dequeue(struct ringbuf *rb,
                   void **obj_table,
                   unsigned n,
                   enum rb_behavior behavior)
{
    uint32_t cons_head, prod_tail, cons_next;
    uint32_t mask, entries;
    unsigned i;

    cons_head = rb->cons.head;
    prod_tail = rb->prod.tail;
    entries = prod_tail - cons_head;
    mask = rb->mask;

    if(entries < n) {
        if(behavior == RB_FIXED)
            return -ENOENT;
        else {
            if(entries == 0)
                return 0;
            n = entries;
        }
    }

    cons_next = cons_head + n;
    rb->cons.head = cons_next;
    DEQUEUE_PTRS();
    __COMPILER_BARRIER();
    rb->cons.tail = cons_next;

    return behavior == RB_FIXED ? 0 : n;
}

/**
 * Enqueue several objects on the ringbuf (multi-producers safe).
 *
 * This function uses a "compare and set" instruction to move the
 * producer index atomically.
 *
 * @param rb
 *   A pointer to the ringbuf structure.
 * @param obj_table
 *   A pointer to a table of void * pointers (objects).
 * @param n
 *   The number of objects to add in the ringbuf from the obj_table.
 * @param behavior
 *   RB_FIXED:    Enqueue a fixed number of items from a ringbuf
 *   RB_VARIABLE: Enqueue as many items as possible from ringbuf
 * @return
 *   Depend on the behavior value
 *   if behavior = RB_FIXED
 *   - 0: Success; objects enqueue.
 *   - -ENOBUFS: Not enough room in the ringbuf to enqueue;
 *     no object is enqueued.
 *   if behavior = RB_VARIABLE
 *   - n: Actual number of objects enqueued.
 */
static inline int
ringbuf_mp_enqueue(struct ringbuf *rb,
                   void *const *obj_table,
                   unsigned n,
                   enum rb_behavior behavior)
{
    uint32_t prod_head, cons_tail, prod_next;
    uint32_t free_entries, mask;
    unsigned i;
    int success;
    const unsigned max = n;

    mask = rb->mask;

    do {
        n = max;
        prod_head = rb->prod.head;
        cons_tail = rb->cons.tail;
        free_entries = mask + cons_tail - prod_head;

        if(free_entries < n) {
            if(behavior == RB_FIXED)
                return -ENOBUFS;
            else {
                if(free_entries == 0)
                    return 0;
                n = free_entries;
            }
        }

        prod_next = prod_head + n;
        success = stl_atomic32_cmpset(&rb->prod.head, prod_head, prod_next);
    } while(success == 0);

    ENQUEUE_PTRS();
    __COMPILER_BARRIER();

    while(rb->prod.tail != prod_head) {};
    rb->prod.tail = prod_next;
    return behavior == RB_FIXED ? 0 : n;
}

/**
 * Dequeue several objects from a ring (multi-consumers safe).
 *
 * When the request objects are more than the available objects,
 * only dequeue the actual number of objects.
 *
 * This function uses a "compare and set" instruction to move the
 * consumer index atomically.
 *
 * @param rb
 *   A pointer to the ringbuf structure.
 * @param obj_table
 *   A pointer to a table of void * pointers (objects) that will be filled.
 * @param n
 *   The number of objects to dequeue from the ringbuf to the obj_table.
 * @param behavior
 *   RB_FIXED:    Dequeue a fixed number of items from a ringbuf
 *   RB_VARIABLE: Dequeue as many items as possible from ringbuf
 * @return
 *   Depend on the behavior value
 *   if behavior = RB_FIXED
 *   - 0: Success; objects dequeued.
 *   - -ENOENT: Not enough entries in the ringbuf to dequeue;
 *     no object is dequeued.
 *   if behavior = RB_VARIABLE
 *   - n: Actual number of objects dequeued.
 */
static inline int
ringbuf_mc_dequeue(struct ringbuf *rb,
                   void **obj_table,
                   unsigned n,
                   enum rb_behavior behavior)
{
    uint32_t cons_head, prod_tail, cons_next;
    uint32_t entries, mask;
    unsigned i;
    int success;
    const unsigned int max = n;

    mask = rb->mask;

    do {
        n = max;
        cons_head = rb->cons.head;
        prod_tail = rb->prod.tail;
        entries = prod_tail - cons_head;

        if(entries < n) {
            if(behavior == RB_FIXED)
                return -ENOENT;
            else {
                if(entries == 0)
                    return 0;
                n = entries;
            }
        }

        cons_next = cons_head + n;
        success = stl_atomic32_cmpset(&rb->cons.head, cons_head, cons_next);
    } while(success == 0);

    DEQUEUE_PTRS();
    __COMPILER_BARRIER();

    while(rb->cons.tail != cons_head) {};
    rb->cons.tail = cons_next;
    return behavior == RB_FIXED ? 0 : n;
}

/**
 * Return the number of entries in a ringbuf.
 *
 * @param rb
 *   A pointer to the ringbuf structure.
 * @return
 *   The number of entries in the ringbuf.
 */
static inline unsigned
ringbuf_count(const struct ringbuf *rb)
{
    uint32_t prod_tail = rb->prod.tail;
    uint32_t cons_tail = rb->cons.tail;
    return (prod_tail - cons_tail) & rb->mask;
}

/**
 * Return the number of free entries in a ringbuf.
 *
 * @param rb
 *   A pointer to the ringbuf structure.
 * @return
 *   The number of free entries in the ringbuf.
 */
static inline unsigned
ringbuf_free_count(const struct ringbuf *rb)
{
    uint32_t prod_tail = rb->prod.tail;
    uint32_t cons_tail = rb->cons.tail;
    return (cons_tail - prod_tail - 1) & rb->mask;
}

/**
 * Test if a ringbuf is full.
 *
 * @param rb
 *   A pointer to the ringbuf structure.
 * @return
 *   - 1: if full.
 *   - 0: if not full.
 */
static inline int
ringbuf_full(const struct ringbuf *rb)
{
    return ringbuf_free_count(rb) == 0;
}

/**
 * Test if a ringbuf is empty.
 *
 * @param rb
 *   A pointer to the ringbuf structure.
 * @return
 *   - 1: if empty.
 *   - 0: if not empty.
 */
static inline int
ringbuf_empty(const struct ringbuf *rb)
{
    uint32_t prod_tail = rb->prod.tail;
    uint32_t cons_tail = rb->cons.tail;
    return prod_tail == cons_tail;
}

END_DECL

#endif // _LIBTINY_RINGBUF_H_
