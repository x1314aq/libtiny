//
//  Created by x1314aq on 2019/1/21.
//

#ifndef _LIBTINY_AVL_H_
#define _LIBTINY_AVL_H_

#include "common.h"


struct avl_node {
    struct avl_node *parent;
    struct avl_node *left;
    struct avl_node *right;
    int height;
};

struct avl_root {
    struct avl_node *node;
};

/// balance factor = left subtree - right subtree

BEGIN_DECL

static inline void
avl_link_node(struct avl_node *n, struct avl_node *p, struct avl_node **avl_link)
{
    n->parent = p;
    n->left = n->right = NULL;
    *avl_link = n;
}

void avl_insert_rotate(struct avl_node *node, struct avl_root *root);

void avl_erase(struct avl_node *node, struct avl_root *root);

void avl_replace_node(struct avl_node *old, struct avl_node *new, struct avl_root *root);

struct avl_node *avl_next(const struct avl_node *node);
struct avl_node *avl_prev(const struct avl_node *node);
struct avl_node *avl_first(const struct avl_root *root);
struct avl_node *avl_last(const struct avl_root *root);

END_DECL

#endif //_LIBTINY_AVL_H_
