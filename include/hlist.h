//
// Created by 叶鑫 on 2018/9/1.
//

/**
 * Hash list implementation.
 * @ref: https://blog.csdn.net/zhanglei4214/article/details/6767288
 */

#ifndef STL_HLIST_H
#define STL_HLIST_H

#include "common.h"


struct hlist_node {
    struct hlist_node *next, **pprev;
};

struct hlist_head {
    struct hlist_node *first;
};

/**
 * Declare a hash list head and initialize to NULL.
 */
#define HLIST_HEAD(name)  \
    struct hlist_head name = { NULL };

/**
 * Delete node from hash list.
 *
 * @param node
 *   Node to be deleted;
 */
static inline void
hlist_del(struct hlist_node *node)
{
    struct hlist_node *next = node->next;
    struct hlist_node **pprev = node->pprev;
    *pprev = next;
    if(next)
        next->pprev = pprev;
    node->next = NULL;
    node->pprev = NULL;
}

/**
 * Add a new node to hash list as the *first* node.
 *
 * @param new
 *   New node to be added.
 * @param head
 *   Hash list head.
 */
static inline void
hlist_add_head(struct hlist_node *new,
               struct hlist_head *head)
{
    struct hlist_node *first = head->first;
    new->next = first;
    if(first)
        first->pprev = &new->next;
    head->first = new;
    new->pprev = &head->first;
}

/**
 * Add a new node to hash list *before* current node.
 *
 * @param new
 *   New node to be added.
 * @param current
 *   Current node before which the new node will be inserted.
 */
static inline void
hlist_add_before(struct hlist_node *new,
                 struct hlist_node *current)
{
    new->next = current;
    new->pprev = current->pprev;
    current->pprev = &new->next;
    *(new->pprev) = new;
}

/**
 * Add a new node to hash list *after* current node.
 *
 * @param new
 *   New node to be added.
 * @param current
 *   Current node after which the new node will be inserted.
 */
static inline void
hlist_add_after(struct hlist_node *new,
                struct hlist_node *current)
{
    new->next = current->next;
    current->next = new;
    new->pprev = &current->next;
    if(new->next)
        new->next->pprev = &new->next;
}

/**
 * Determine whether a hash list is empty.
 *
 * @param head
 *   Hash list head to be judged.
 * @return
 *   - 1 if empty.
 *   - 0 if not empty.
 */
static inline int
hlist_empty(struct hlist_head *head)
{
    return !head->first;
}

/**
 * Macro used to retrieve address of structure *type*
 *
 * @param ptr
 *   Pointer to hlist node field in structure *type*.
 * @param type
 *   Structure which contains list_entry field.
 * @return
 *   Pointer to structure *type*.
 */
#define HLIST_ENTRY(ptr, type)    container_of(ptr, type, hlist_node)

/**
 * Macro used to *reversely traverse* the whole list.
 *
 * @param pos
 *   Pointer to access each hash list node.
 * @param head
 *   Head of the hash list to be traversed.
 */
#define HLIST_FOR_EACH(pos, head)  \
    for(pos = (head)->first; pos; pos = pos->next)

#endif //STL_HLIST_H
