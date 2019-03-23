//
//  Created by x1314aq on 2019/2/2.
//

#include "avl_map.h"


static uint64_t test_hash(const void *key)
{
    return (uint64_t) key;
}

static int test_cmp(const void *key1, const void *key2)
{
    if(key1 == key2)
        return 0;
    return (uint64_t) key1 < (uint64_t) key2 ? -1 : 1;
}

static inline void print_usage(int code)
{
    puts("Usage: avl_map_test -n <NUMBER>   default: 1000000");
    exit(code);
}

int main(int argc, char *argv[])
{
    int err = 0;
    int i, c, number = 1000000;
    uint64_t k, v;
    struct map_type type = {
        .hash_func = test_hash,
        .key_compare = test_cmp
    };
    struct map test_map;

    while((c = getopt(argc, argv, "hn:")) != -1) {
        switch(c) {
            case 'n':
                number = (int) strtol(optarg, NULL, 10);
                break;
            case 'h':
            case '?':
                print_usage(0);
            default:
                print_usage(1);
        }
    }

    if(number < 0) {
        fprintf(stderr, "negative entry NUMBER: %d\n", number);
        exit(1);
    }

    if(avl_map_init(&test_map, &type)) {
        fprintf(stderr, "init failed!");
        exit(1);
    }

    for(i = 0; i < number; i++) {
        k = i;
        v = i << 1;
        avl_map_insert(&test_map, (void *) k, (void *) v);
    }

    for(i = 0; i < number; i += 2) {
        k = i;
        struct map_entry *e = avl_map_find(&test_map, (void *) k);
        if(e) {
            v = (uint64_t) e->val;
            if(v != k << 1) {
                fprintf(stderr, "error1 in avl_map_find at %d\n", i);
                err = 1;
            }
            avl_map_erase(&test_map, e);
        }
    }

    for(i = 0; i < number; i += 2) {
        k = i;
        struct map_entry *e = avl_map_find(&test_map, (void *) k);
        if(e) {
            fprintf(stderr, "error2 in avl_map_find at %d\n", i);
            err = 1;
        }
    }

    avl_map_destroy(&test_map);
    return err;
}
