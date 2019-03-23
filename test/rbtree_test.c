//
//  Created by x1314aq on 2019/1/20.
//

#include "rbtree.h"

struct test_node {
    uint32_t key;
    struct rb_node rb;
};

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

static inline void print_usage(int code)
{
    puts("Usage: rbtree_test -n <NUMBER>   default: 1000000");
    exit(code);
}

int main(int argc, char *argv[])
{
    int err = 0;
    int i, c, number = 1000000;
    struct test_node *test, *nodes;
    struct rb_node *node;
    struct rb_root root = { NULL };

    while((c = getopt(argc, argv, "hn:")) != -1) {
        switch(c) {
            case 'n':
                number = atoi(optarg);
                break;
            case 'h':
            case '?':
                print_usage(0);
            default:
                print_usage(1);
        }
    }

    if(number < 0) {
        fprintf(stderr, "negative node NUMBER: %d\n", number);
        exit(1);
    }

    nodes = (struct test_node *) calloc(number, sizeof(struct test_node));
    uint32_t *temp = (uint32_t *) malloc(number * sizeof(uint32_t));
    int k = 0; 

    for(i = 0; i < number; i++) {
        nodes[i].key = rand();
        insert(&nodes[i], &root);
        if(rb_is_red(root.node)) {
            fprintf(stderr, "insert error at %d\n", i);
            err = 1;
            goto END;
        }
    }

    for(node = rb_first(&root); node; node = rb_next(node)) {
        test = container_of(node, struct test_node, rb);
        temp[k++] = test->key;
    }

    for(node = rb_last(&root); node; node = rb_prev(node)) {
        test = container_of(node, struct test_node, rb);
        temp[--k] -= test->key;
        if(temp[k] != 0) {
            fprintf(stderr, "error at %d\n", k);
            err = 1;
        }
    }

    for(i = 0; i < number - 1; i++) {
        erase(&nodes[i], &root);
        if(rb_is_red(root.node)) {
            fprintf(stderr, "erase error at %d\n", i);
            err = 1;
            goto END;
        }
    }

    if(root.node != &nodes[number - 1].rb) {
        fprintf(stderr, "error\n");
        err = 1;
    }

END:
    free(temp);
    free(nodes);
    return err;
}
