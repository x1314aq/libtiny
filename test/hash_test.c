//
//  Created by x1314aq on 2019/3/2
//

#include "hash.h"

HASH_DECLARE(test, int, int);

HASH_IMPL(test, int, int, default_int32_hash_func, default_int32_hash_equal);

static inline void print_usage()
{
    puts("Usage: hash_test -n <NUMBER>   default: 1000000");
}

int main(int argc, char *argv[])
{
    int i, c, number = 1000000;
    int k, v;

    while((c = getopt(argc, argv, "hn:")) != -1) {
        switch(c) {
            case 'n':
                number = atoi(optarg);
                break;
            case 'h':
            case '?':
            default:
                print_usage();
                return 0;
        }
    }

    if(number < 0) {
        printf("nagetive entry NUMBER: %d\n", number);
        return 1;
    }

    hash_t(test) *h = hash_init(test);
    if(!h) {
        puts("init failed!");
        return 1;
    }

    for(i = 0; i < number; i++) {
        k = i;
        v = i << 1;
        int ret = hash_insert(test, h, k ,v);
        if(ret) {
            printf("insert failed! errno=%d\n", ret);
            goto DONE;
        }
    }

    if(h->size != (uint32_t)number) {
        puts("error 1!");
        goto DONE;
    }

    for(i = 0; i < number; i++) {
        k = i;
        uint32_t iter = hash_find(test, h, k);
        hash_erase(test, h, iter);
    }

    if(h->size != 0) {
        puts("error 2!");
        goto DONE;
    }

DONE:
    hash_clear(test, h);
    hash_destroy(test, h);
    return 0;
}
