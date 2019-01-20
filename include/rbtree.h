//
//  Created by x1314aq on 2019/1/19.
//

#ifndef _LIBTINY_RBTREE_H_
#define _LIBTINY_RBTREE_H_

#include "common.h"


struct rb_node {
    unsigned long parent_color;
    struct rb_node *left;
    struct rb_node *right;
};

struct rb_root {
    struct rb_node *node;
};

#define RB_RED             0
#define RB_BLACK           1
#define __rb_color(pc)     ((pc) & 1)
#define rb_is_black(n)     (__rb_color((n)->parent_color))
#define rb_is_red(n)       (!rb_is_black(n))
#define rb_parent(n)       ((struct rb_node *)((n)->parent_color & ~3))

BEGIN_DECL

/**
 * Make *p* the parent of *n*.
 *
 * @param n
 *   The rb_node to be the child.
 * @param p
 *   The rb_node to be the parent.
 */
static inline void
rb_set_parent(struct rb_node *n, struct rb_node *p)
{
    n->parent_color = __rb_color(n->parent_color) | (unsigned long) p;
}

/**
 * Make *p* the parent of *n* & set color of *n* to *color*.
 *
 * @param n
 *   The rb_node to be the child and to be re-colored.
 * @param p
 *   The rb_node to be the parent.
 * @param color
 *   The color to color *n*
 */
static inline void
rb_set_parent_color(struct rb_node *n, struct rb_node *p, int color)
{
    n->parent_color = (unsigned long) p | color;
}

/**
 * Insert *n* to the position of *rb_link* and set its parent to *p*.
 *
 * Note that *n* is inserted as a red rb_node.
 *
 * @param n
 *   The rb_node to be linked.
 * @param p
 *   The rb_node to be the parent.
 * @param rb_link
 *   The pointer to which *n* will be linked.
 */
static inline void
rb_link_node(struct rb_node *n, struct rb_node *p, struct rb_node **rb_link)
{
    n->parent_color = (unsigned long) p;
    n->left = n->right = NULL;
    *rb_link = n;
}

void rb_insert_color(struct rb_node *, struct rb_root *);

void rb_erase(struct rb_node *, struct rb_root *);

void rb_replace_node(struct rb_node *, struct rb_node *, struct rb_root *);

struct rb_node *rb_next(const struct rb_node *);
struct rb_node *rb_prev(const struct rb_node *);
struct rb_node *rb_first(const struct rb_root *);
struct rb_node *rb_last(const struct rb_root *);

END_DECL

#endif // _LIBTINY_RBTREE_H_
