//
//  Created by x1314aq on 2017/7/22.
//

#include "rbtree.h"

static inline void __rb_set_black(struct rb_node *n)
{
    n->parent_color |= RB_BLACK;
}

void rb_insert_color(struct rb_node *n, struct rb_root *root)
{
    /// parent, grandparent, uncle, sibling, great-grandparent
    struct rb_node *p, *g, *u, *s, *gg;

    p = rb_parent(n);
    while(1) {
        /**
         * If parent is NULL, either this is the first inserted node,
         * or we recursed at Case 1 below.
         */
        if(unlikely(!p)) {
            rb_set_parent_color(n , NULL, RB_BLACK);
            break;
        }

        /**
         * If parent is black, then we are done.
         */
        if(rb_is_black(p))
            break;

        /**
         * Parent is red, thus grandparent is definitely black.
         */
        g = rb_parent(p);
        if(p == g->left) {
            u = g->right;
            if(u && rb_is_red(u)) {
                /**
                 * Uppercase letter is black, while lowercase is red.
                 * Case 1:
                 *      G                g
                 *     / \             /   \
                 *   p     u  ---->   P      U
                 *  /                /
                 * n                n
                 */
                rb_set_parent_color(p, g, RB_BLACK);
                rb_set_parent_color(u, g, RB_BLACK);
                n = g;
                p = rb_parent(n);
                rb_set_parent_color(n, p, RB_RED);
                continue;
            }

            /**
             * Uncle is definitely black.
             */
            if(n == p->left) {
                /**
                 * Case 2:
                 *     G             P
                 *    / \           / \
                 *   p   U  ---->  n   g
                 *  / \               / \
                 * n   S             S   U
                 */
                s = p->right;
                g->left = s;
                rb_set_parent(s, g);
                p->right = g;
                gg = rb_parent(g);
                rb_set_parent_color(p, gg, RB_BLACK);
                if(likely(gg)) {
                    if(gg->left == g)
                        gg->left = p;
                    else
                        gg->right = p;
                }
                else
                    root->node = p;
                rb_set_parent_color(g, p, RB_RED);
                break;
            }

            /**
             * Case 3:
             *     G              N
             *    / \            / \
             *   p   U  ---->   p   g
             *  / \            /     \
             * S   n          S       U
             */
            p->right = n->left;
            rb_set_parent(n->left, p);
            n->left = p;
            rb_set_parent(p, n);
            gg = rb_parent(g);
            rb_set_parent_color(n, gg, RB_BLACK);
            if(likely(gg)) {
                if(gg->left == g)
                    gg->left = n;
                else
                    gg->right = n;
            }
            else
                root->node = n;
            g->left = n->right;
            rb_set_parent(n->right, g);
            n->right = g;
            rb_set_parent_color(g, n, RB_RED);
        }
        else {  /// p == g->right
            u = g->left;
            if(u && rb_is_red(u)) {
                rb_set_parent_color(p, g, RB_BLACK);
                rb_set_parent_color(u, g, RB_BLACK);
                n = g;
                p = rb_parent(n);
                rb_set_parent_color(n, p, RB_RED);
                continue;
            }

            if(n == p->right) {
                s = p->left;
                g->right = s;
                rb_set_parent(s, g);
                p->left = g;
                gg = rb_parent(g);
                rb_set_parent_color(p, gg, RB_BLACK);
                if(likely(gg)) {
                    if(gg->left == g)
                        gg->left = p;
                    else
                        gg->right = p;
                }
                else
                    root->node = p;
                rb_set_parent_color(g, p, RB_RED);
                break;
            }

            p->left = n->right;
            rb_set_parent(n->right, p);
            n->right = p;
            rb_set_parent(p, n);
            gg = rb_parent(g);
            rb_set_parent_color(n, gg, RB_BLACK);
            if(likely(gg)) {
                if(gg->left == g)
                    gg->left = n;
                else
                    gg->right = n;
            }
            else
                root->node = n;
            g->right = n->left;
            rb_set_parent(n->left, g);
            n->left = g;
            rb_set_parent_color(g, n, RB_RED);
        }
    }
}

static inline void __rb_erase_color(struct rb_node *p, struct rb_root *root)
{
    struct rb_node *n, *s, *c1, *c2, *g;
    n = NULL;

    while(1) {
        s = p->right;
        if(n != s) {
            if(rb_is_red(s)) {
                /**
                 *     P               S
                 *    / \             / \
                 *   N   s   ---->   p   C2
                 *      / \         / \
                 *     C1 C2       N  C1
                 */
                c1 = s->left;
                g = rb_parent(p);
                p->right = c1;
                rb_set_parent_color(c1, p, RB_BLACK);
                s->left = p;
                rb_set_parent_color(p, s, RB_RED);
                if(likely(g)) {
                    if(g->left == p)
                        g->left = s;
                    else
                        g->right = s;
                }
                else
                    root->node = s;
                rb_set_parent_color(s, g, RB_BLACK);
                s = c1;
            }
            c1 = s->right;
            if(!c1 || rb_is_black(c1)) {
                c2 = s->left;
                if(!c2 || rb_is_black(c2)) {
                    /**
                     * 1. both c1 and c2 are NULL;
                     * 2. c1 is NULL, but c2 exists and is black;
                     * 3. c1 exists and is black, but c2 is NULL;
                     * 4. both c1 and c2 exist and are black;
                     *
                     *    (p)             (p)
                     *    / \             / \
                     *   N   S    ---->  N   s
                     *      / \             / \
                     *     C2  C1          C2 C1
                     */
                    rb_set_parent_color(s, p, RB_RED);
                    if(rb_is_red(p))
                        __rb_set_black(p);
                    else {
                        n = p;
                        p = rb_parent(n);
                        if(p)
                            continue;
                    }
                    break;
                }
                /**
                 * 1. c1 is NULL, but c2 exists and is red;
                 * 2. c1 exists and is black, while c2 exists and is red;
                 *
                 *    (p)             (p)
                 *    / \             / \
                 *   N   S    ---->  N   c2
                 *      / \               \
                 *     c2  C1              S
                 *                          \
                 *                           C1
                 */
                c1 = c2->right;
                s->left = c1;
                c2->right = s;
                p->right = c2;
                if(c1)
                    rb_set_parent_color(c1, s, RB_BLACK);
                c1 = s;
                s = c2;
            }
            /**
             * c1 exists and is red.
             *
             *     (p)             (s)
             *     / \             / \
             *    N   S    ---->  P   C1
             *       / \         / \
             *     (c2) c1      N  (c2)
             */
            c2 = s->left;
            p->right = c2;
            s->left = p;
            rb_set_parent_color(c1, s, RB_BLACK);
            if(c2)
                rb_set_parent(c2, p);
            g = rb_parent(p);
            s->parent_color = p->parent_color;
            if(likely(g)) {
                if(g->left == p)
                    g->left = s;
                else
                    g->right = s;
            }
            else
                root->node = s;
            rb_set_parent_color(p, s, RB_BLACK);
            break;
        }
        else {
            s = p->left;
            if(rb_is_red(s)) {
                c1 = s->right;
                g = rb_parent(p);
                p->left = c1;
                rb_set_parent_color(c1, p, RB_BLACK);
                s->right = p;
                rb_set_parent_color(p, s, RB_RED);
                if(likely(g)) {
                    if(g->left == p)
                        g->left = s;
                    else
                        g->right = s;
                }
                else
                    root->node = s;
                rb_set_parent_color(s, g, RB_BLACK);
                s = c1;
            }
            c1 = s->left;
            if(!c1 || rb_is_black(c1)) {
                c2 = s->right;
                if(!c2 || rb_is_black(c2)) {
                    rb_set_parent_color(s, p, RB_RED);
                    if(rb_is_red(p))
                        __rb_set_black(p);
                    else {
                        n = p;
                        p = rb_parent(n);
                        if(p)
                            continue;
                    }
                    break;
                }
                c1 = c2->left;
                s->right = c1;
                c2->left = s;
                p->left = c2;
                if(c1)
                    rb_set_parent_color(c1, s, RB_BLACK);
                c1 = s;
                s = c2;
            }
            c2 = s->right;
            p->left = c2;
            s->right = p;
            rb_set_parent_color(c1, s, RB_BLACK);
            if(c2)
                rb_set_parent(c2, p);
            g = rb_parent(p);
            s->parent_color = p->parent_color;
            if(likely(g)) {
                if(g->left == p)
                    g->left = s;
                else
                    g->right = s;
            }
            else
                root->node = s;
            rb_set_parent_color(p, s, RB_BLACK);
            break;
        }
    }
}

void rb_erase(struct rb_node *n, struct rb_root *root)
{
    struct rb_node *c, *p;

    if(!n->left) {
        p = rb_parent(n);
        c = n->right;
        if(likely(p)) {
            if(p->left == n)
                p->left = c;
            else
                p->right = c;
        }
        else
            root->node = c;
        if(c)     /// n has only one child, so n is black, the child is red.
            c->parent_color = n->parent_color;
        else      /// n has no child
            if(rb_is_black(n))   /// if n is black, then need rebalance, else n is red and everything is fine.
                __rb_erase_color(p, root);
    }
    else if(!n->right) {   /// n has one left child.
        p = rb_parent(n);
        c = n->left;
        if(likely(p)) {
            if(p->left == n)
                p->left = c;
            else
                p->right = c;
        }
        else
            root->node = c;
        c->parent_color = n->parent_color;
    }
    else {
        struct rb_node *successor, *child, *parent;
        p = rb_parent(n);
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
            rb_set_parent(child, parent);
        }
        successor->left = n->left;
        rb_set_parent(n->left, successor);
        if(likely(p)) {
            if(p->left == n)
                p->left = successor;
            else
                p->right = successor;
        }
        else
            root->node = successor;
        if(child) {
            successor->parent_color = n->parent_color;
            rb_set_parent_color(child, parent, RB_BLACK);
        }
        else {
            unsigned long pc = successor->parent_color;
            successor->parent_color = n->parent_color;
            if(__rb_color(pc))    /// if successor is a black node
                __rb_erase_color(parent, root);
        }
    }
}

struct rb_node *rb_first(const struct rb_root *root)
{
    struct rb_node *n;
    n = root->node;

    if(!n)
        return NULL;
    while(n->left)
        n = n->left;
    return n;
}

struct rb_node *rb_last(const struct rb_root *root)
{
    struct rb_node *n;
    n = root->node;

    if(!n)
        return NULL;
    while(n->right)
        n = n->right;
    return n;
}

struct rb_node *rb_next(const struct rb_node *n)
{
    struct rb_node *p;

    if(n->right) {
        n = n->right;
        while(n->left)
            n = n->left;
        return (struct rb_node *)n;
    }

    while((p = rb_parent(n)) && n == p->right)
        n = p;

    return p;
}

struct rb_node *rb_prev(const struct rb_node *n)
{
    struct rb_node *p;

    if(n->left) {
        n = n->left;
        while(n->right)
            n = n->right;
        return (struct rb_node *)n;
    }

    while((p = rb_parent(n)) && n == p->left)
        n = p;

    return p;
}

void rb_replace_node(struct rb_node *old, struct rb_node *new, struct rb_root *root)
{
    struct rb_node *p = rb_parent(old);

    new->left = old->left;
    new->right = old->right;
    new->parent_color = old->parent_color;

    if(old->left)
        rb_set_parent(old->left, new);
    if(old->right)
        rb_set_parent(old->right, new);

    if(likely(p)) {
        if(p->left == old)
            p->left = new;
        else
            p->right = new;
    }
    else
        root->node = new;
}
