//
//  Created by x1314aq on 2019/1/20.
//

#include "rbtree.h"

struct test_node {
    uint32_t key;
    struct rb_node rb;
};

#define NUMBER    10000

static struct test_node nodes[NUMBER];

static void insert(struct test_node *node, struct rb_root *root)
{
    struct rb_node **new = &root->node;
    struct rb_node *parent = NULL;
    uint32_t key = node->key;
    struct test_node *current;

    while(*new) {
        parent = *new;
        current = container_of(parent, struct test_node, rb);
        if(key < current->key)
            new = &parent->left;
        else
            new = &parent->right;
    }

    rb_link_node(&node->rb, parent, new);
    rb_insert_color(&node->rb, root);
}

static inline void erase(struct test_node *node, struct rb_root *root)
{
    rb_erase(&node->rb, root);
}

int main(int argc, char *argv[])
{
    int i;
    struct test_node *test;
    struct rb_node *node;
    struct rb_root root = { NULL };

    for(i = 0; i < NUMBER; i++) {
        nodes[i].key = rand();
        printf("%u ", nodes[i].key);
        insert(&nodes[i], &root);
    }

    for(node = rb_first(&root); node; node = rb_next(node)) {
        test = container_of(node, struct test_node, rb);
        printf("%u ", test->key);
    }

    for(node = rb_last(&root); node; node = rb_prev(node)) {
        test = container_of(node, struct test_node, rb);
        printf("%u ", test->key);
    }

    for(i = 0; i < NUMBER; i++)
        erase(&nodes[i], &root);

    return 0;
}
