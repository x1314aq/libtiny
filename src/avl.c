//
//  Created by x1314aq on 2017/7/22.
//

#include "avl.h"

#define avl_height(n)    ((n) ? (n)->height : 0)

static inline struct avl_node *
__avl_rotate_r(struct avl_node *n, struct avl_root *root)
{
    int hl, hr;
    struct avl_node *g, *p, *v, *vl, *pr;

    p = n->left;
    hl = avl_height(p->left);
    hr = avl_height(p->right);
    if(hl < hr) {
        /**
         *     n           n
         *    /   zag     /
         *   p   ---->   v
         *    \         / \
         *     v       p  vr
         *    / \       \
         *   vl vr       vl
         */
        v = p->right;
        vl = v->left;
        p->right = vl;
        if(vl)
            vl->parent = p;
        v->left = p;
        p->parent = v;
        n->left = v;
        v->parent = n;
        p = v;
    }
    /**
     *       n            p
     *      /    zig     / \
     *     p    ---->   v   n
     *    / \              /
     *   v   pr           pr
     */
    v = p->left;
    pr = p->right;
    g = n->parent;
    n->left = pr;
    if(pr)
        pr->parent = n;
    n->parent = p;
    p->right = n;
    p->parent = g;
    if(likely(g)) {
        if(g->left == n)
            g->left = p;
        else
            g->right = p;
    }
    else
        root->node = p;
    return p;
}

static inline struct avl_node *
__avl_rotate_l(struct avl_node *n, struct avl_root *root)
{
    int hl, hr;
    struct avl_node *g, *p, *v, *vr, *pl;

    p = n->right;
    hl = avl_height(p->left);
    hr = avl_height(p->right);
    if(hl > hr) {
        /**
         *    n             n
         *     \      zig    \
         *      p    ---->    v
         *     / \             \
         *    v  pr             p
         *     \               / \
         *     vr             vr pr
         */
        v = p->left;
        vr = v->right;
        p->left = vr;
        if(vr)
            vr->parent = p;
        v->right = p;
        p->parent = v;
        n->right = v;
        v->parent = n;
        p = v;
    }
    /**
     *   n               p
     *    \      zag    / \
     *     p    ---->  n   v
     *    / \           \
     *   pl  v           pl
     */
    v = p->right;
    pl = p->left;
    g = n->parent;
    n->right = pl;
    if(pl)
        pl->parent = n;
    n->parent = p;
    p->left = n;
    p->parent = g;
    if(likely(g)) {
        if(g->left == n)
            g->left = p;
        else
            g->right = p;
    }
    else
        root->node = p;
    return p;
}

void avl_insert_rotate(struct avl_node *n, struct avl_root *root)
{
    int hl, hr, height;
    n->height = 1;

    for(n = n->parent; n; n = n->parent) {
        hl = avl_height(n->left);
        hr = avl_height(n->right);
        height = MAX(hl, hr) + 1;
        if(height == n->height)
            break;
        n->height = height;
        if(hl - hr < -1)
            n = __avl_rotate_l(n, root);
        else if(hl - hr > 1)
            n = __avl_rotate_r(n, root);
    }
}

static inline void
__avl_erase_rebalance(struct avl_node *n, struct avl_root *root)
{
    int hl, hr, height, diff;

    while(n) {
        hl = avl_height(n->left);
        hr = avl_height(n->right);
        height = MAX(hl, hr) + 1;
        diff = hl - hr;
        if(n->height != height)
            n->height = height;
        else if(diff >= -1 && diff <= 1)
            break;

        if(diff < -1)
            n = __avl_rotate_l(n, root);
        else if(diff > 1)
            n = __avl_rotate_r(n, root);
        n = n->parent;
    }
}

void avl_erase(struct avl_node *n, struct avl_root *root)
{
    struct avl_node *c, *p;

    if(!n->left) {
        c = n->right;
        p = n->parent;
        if(likely(p)) {
            if(p->left == n)
                p->left = c;
            else
                p->right = c;
        }
        else
            root->node = c;
        if(c)
            c->parent = p;
    }
    else if(!n->right) {
        c = n->left;
        p = n->parent;
        if(likely(p)) {
            if(p->left == n)
                p->left = c;
            else
                p->right = c;
        }
        else
            root->node = c;
        c->parent = p;
    }
    else {
        struct avl_node *successor, *child, *parent;
        p = n->parent;
        c = n->right;

        if(!c->left) {
            successor = c;
            parent = successor;
            child = successor->right;
        }
        else {
            successor = c->left;
            parent = c;
            while(successor->left) {
                parent = successor;
                successor = parent->left;
            }
            child = successor->right;
            parent->left = child;
            successor->right = c;
            c->parent = successor;
        }
        successor->left = n->left;
        n->left->parent = successor;
        if(likely(p)) {
            if(p->left == n)
                p->left = successor;
            else
                p->right = successor;
        }
        else
            root->node = successor;
        successor->parent = p;
        if(child)
            child->parent = parent;
        p = parent;
    }

    if(p)
        __avl_erase_rebalance(p, root);
}

struct avl_node *avl_first(const struct avl_root *root)
{
    struct avl_node *n;
    n = root->node;

    if(!n)
        return NULL;
    while(n->left)
        n = n->left;
    return n;
}

struct avl_node *avl_last(const struct avl_root *root)
{
    struct avl_node *n;
    n = root->node;

    if(!n)
        return NULL;
    while(n->right)
        n = n->right;
    return n;
}

struct avl_node *avl_next(const struct avl_node *n)
{
    struct avl_node *p;

    if(n->right) {
        n = n->right;
        while(n->left)
            n = n->left;
        return (struct avl_node *)n;
    }

    while((p = n->parent) && n == p->right)
        n = p;

    return p;
}

struct avl_node *avl_prev(const struct avl_node *n)
{
    struct avl_node *p;

    if(n->left) {
        n = n->left;
        while(n->right)
            n = n->right;
        return (struct avl_node *)n;
    }

    while((p = n->parent) && n == p->left)
        n = p;

    return p;
}

void avl_replace_node(struct avl_node *old, struct avl_node *new, struct avl_root *root)
{
    struct avl_node *p = old->parent;

    new->left = old->left;
    new->right = old->right;
    new->parent = old->parent;
    new->height = old->height;

    if(old->left)
        old->left->parent = new;
    if(old->right)
        old->right->parent = new;

    if(likely(p)) {
        if(p->left == old)
            p->left = new;
        else
            p->right = new;
    }
    else
        root->node = new;
}
