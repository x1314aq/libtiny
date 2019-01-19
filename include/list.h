//
// Created by x1314aq on 2018/8/28.
//

/**
 * Double linked list implementation.
 *
 * @ref: https://www.ibm.com/developerworks/cn/linux/kernel/l-chain/index.html
 */

#ifndef _LIBTINY_LIST_H_
#define _LIBTINY_LIST_H_

#include "common.h"


struct list_entry {
    struct list_entry *prev, *next;
};

BEGIN_DECL

/**
 * Declare a list head and initialize to empty.
 */
#define LIST_HEAD(name)  \
    struct list_entry name = {&(name), &(name)};

/**
 * Runtime initialization for list head.
 */
static inline void
list_head_init(struct list_entry *head)
{
    head->next = head;
    head->prev = head;
}

/**
 * Internal list manipulation.
 *
 * @param new
 *   List entry to be inserted.
 * @param prev
 *   List entry before *new*.
 * @param next
 *   List entry after *new*.
 */
static inline void
__list_add(struct list_entry *new,
           struct list_entry *prev,
           struct list_entry *next)
{
    prev->next = new;
    new->prev = prev;
    new->next = next;
    next->prev = new;
}

/**
 * Insert a new list entry *after* current position.
 *
 * @param new
 *   List entry to be inserted.
 * @param cur
 *   Current position after which *new* will be inserted.
 */
static inline void
list_add_after(struct list_entry *new,
               struct list_entry *cur)
{
    __list_add(new, cur, cur->next);
}

/**
 * Insert a new list entry *before* current position.
 *
 * @param new
 *   List entry to be inserted.
 * @param cur
 *   Current position before which *new* will be inserted.
 */
static inline void
list_add_before(struct list_entry *new,
                struct list_entry *cur)
{
    __list_add(new, cur->prev, cur);
}

/**
 * Internal list manipulation, range deletion between (prev, next).
 *
 * @param prev
 *   Start point of delete operation.
 * @param next
 *   End point of delete operation.
 */
static inline void
__list_del(struct list_entry *prev,
           struct list_entry *next)
{
    prev->next = next;
    next->prev = prev;
}

/**
 * Delete a list entry and initialize to empty.
 *
 * @param entry
 *   List entry to be deleted.
 */
static inline void
list_delete(struct list_entry *entry)
{
    __list_del(entry->prev, entry->next);
    list_head_init(entry);
}

/**
 * Determine whether a list is empty.
 *
 * @param head
 *   List head to be judged.
 * @return
 *   - 1 if empty.
 *   - 0 if not empty.
 */
static inline int
list_empty(struct list_entry *head)
{
    return head->next == head;
}

/**
 * Macro used to *traverse* the whole list.
 *
 * @param pos
 *   Pointer to access each list entry.
 * @param head
 *   List head to be traversed.
 */
#define LIST_FOR_EACH(pos, head)  \
    for(pos = (head)->next; pos != (head); pos = pos->next)

/**
 * Macro used to *reversely traverse* the whole list.
 *
 * @param pos
 *   Pointer to access each list entry.
 * @param head
 *   List head to be traversed.
 */
#define LIST_FOR_EACH_REVERSE(pos, head)  \
    for(pos = (head)->prev; pos != (head); pos = pos->prev)

END_DECL

#endif //_LIBTINY_LIST_H_
