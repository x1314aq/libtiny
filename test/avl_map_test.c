//
//  Created by x1314aq on 2019/2/2.
//

#include "avl_map.h"


#define NUM_ENTRIES    1000000

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

int main(int argc, char *argv[])
{
    uint64_t k, v;
    struct map_type type = {
        .hash_func = test_hash,
        .key_compare = test_cmp
    };
    struct map test_map;

    if(avl_map_init(&test_map, &type)) {
        puts("init failed!");
        return -1;
    }

    for(int i = 0; i < NUM_ENTRIES; i++) {
        k = i;
        v = i << 1;
        avl_map_insert(&test_map, (void *) k, (void *) v);
    }

    for(int i = 0; i < NUM_ENTRIES; i += 2) {
        k = i;
        struct map_entry *e = avl_map_find(&test_map, (void *) k);
        if(e) {
            v = (uint64_t) e->val;
            if(v != k << 1) {
                puts("Error1!");
                return -1;
            }
            avl_map_erase(&test_map, e);
        }
    }

    for(int i = 0; i < NUM_ENTRIES; i += 2) {
        k = i;
        struct map_entry *e = avl_map_find(&test_map, (void *) k);
        if(e) {
            puts("Error2!");
            return -1;
        }
    }

    avl_map_destroy(&test_map);
    return 0;
}
