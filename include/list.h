//
// Created by 叶鑫 on 2018/8/28.
//

/**
 * Double linked list implementation.
 *
 * @ref: https://www.ibm.com/developerworks/cn/linux/kernel/l-chain/index.html
 */

#ifndef STL_LIST_H
#define STL_LIST_H

#include "common.h"


struct list_entry {
    struct list_entry *prev, *next;
};


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
 * Insert a new list entry *new* after list head.
 *
 * @param new
 *   List entry to be inserted.
 * @param head
 *   List head after which *new* will be inserted.
 */
static inline void
list_add_head(struct list_entry *new,
              struct list_entry *head)
{
    __list_add(new, head, head->next);
}

/**
 * Insert a new list entry *new* before list tail.
 *
 * @param new
 *   List entry to be inserted.
 * @param head
 *   List tail (.aka. head->prev) before which *new* will be inserted.
 */
static inline void
list_add_tail(struct list_entry *new,
              struct list_entry *head)
{
    __list_add(new, head->prev, head);
}

/**
 * Internal list manipulation. Range deletion between (prev, next).
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
list_del_entry(struct list_entry *entry)
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
 * Macro used to retrieve address of structure *type*.
 *
 * @param ptr
 *   Pointer to list_entry field in structure *type*;
 * @param type
 *   Structure which contains list_entry field;
 * @return
 *   Pointer to structure *type*
 */
#define LIST_ENTRY(ptr, type)  container_of(prt, type, list_entry)

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

#endif //STL_LIST_H
