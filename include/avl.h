//
//  Created by x1314aq on 2019/1/21.
//

#ifndef _LIBTINY_AVL_H_
#define _LIBTINY_AVL_H_

#include "common.h"


struct avl_node {
    unsigned long parent_balance;
    struct avl_node *left;
    struct avl_node *right;
};

struct avl_root {
    struct avl_node *node;
};

/// balance factor = left subtree - right subtree

#define avl_is_balanced(n)     (((n)->parent_balance & 3) == 0)
#define avl_left_dominant(n)   (((n)->parent_balance & 3) == 1)
#define avl_right_dominant(n)  (((n)->parent_balance & 3) == 3)

BEGIN_DECL

static inline void
avl_link_node(struct avl_node *n, struct avl_node *p, struct avl_node **avl_link)
{
    n->parent_balance = (unsigned long) p;
    n->left = n->right = NULL;
    *avl_link = n;
}

void avl_insert_rotate(struct avl_node *, struct avl_root *);

void avl_erase(struct avl_node *, struct avl_root *);

void avl_replace_node(struct avl_node *, struct avl_node *, struct avl_root *);

struct avl_node *avl_next(struct avl_node *);
struct avl_node *avl_prev(struct avl_node *);
struct avl_node *avl_first(struct avl_root *);
struct avl_node *avl_last(struct avl_root *);

END_DECL

#endif //_LIBTINY_AVL_H_
