//
//  Created by x1314aq on 2019/3/2
//

#include "hash.h"

// 32-bit integer key with default int32 hash function
HASH_DECLARE(test1, int, int);
HASH_IMPL(test1, int, int, default_int32_hash_func, default_int32_hash_equal);

// 32-bit integer key with alternative int32 hash function
HASH_DECLARE(test2, int, int);
HASH_IMPL(test2, int, int, alter_int32_hash_func, default_int32_hash_equal);

// 64-bit integer key with default int64 hash function
HASH_DECLARE(test3, uint64_t, int);
HASH_IMPL(test3, uint64_t, int, default_int64_hash_func, default_int64_hash_equal);

// string key with default string hash function
HASH_DECLARE(test4, char *, int);
HASH_IMPL(test4, char *, int, default_string_hash_func, default_string_hash_equal);


static int test1(int number)
{
    int ret = 0;
    int k, v, i;
    hash_t(test1) *h = hash_init(test1);
    if(!h) {
        return 1;
    }

    for(i = 0; i < number; i++) {
        k = i;
        v = i << 1;
        ret = hash_insert(test1, h, k ,v);
        if(ret < 0)
            goto DONE;
    }

    if(hash_size(test1, h) != (uint32_t)number) {
        ret = 2;
        goto DONE;
    }

    for(i = 0; i < number; i++) {
        k = i;
        v = i << 1;
        hash_iter_t iter = hash_find(test1, h, k);
        if(hash_valid(test1, h, iter)) {
            if(hash_get_key(test1, h, iter) != k || hash_get_val(test1, h, iter) != v) {
                ret = 4;
                goto DONE;
            }
            hash_erase(test1, h, iter);
        }
        else {
            ret = 5;
            goto DONE;
        }
    }

    if(!hash_empty(test1, h)) {
        ret = 3;
        goto DONE;
    }

DONE:
    hash_clear(test1, h);
    hash_destroy(test1, h);
    return ret;
}

static int test2(int number)
{
    int ret = 0;
    int k, v, i;
    hash_t(test2) *h = hash_init(test2);
    if(!h) {
        return 1;
    }

    for(i = 0; i < number; i++) {
        k = i;
        v = i << 1;
        ret = hash_insert(test2, h, k ,v);
        if(ret < 0)
            goto DONE;
    }

    if(hash_size(test2, h) != (uint32_t)number) {
        ret = 2;
        goto DONE;
    }

    for(i = 0; i < number; i++) {
        k = i;
        v = i << 1;
        hash_iter_t iter = hash_find(test2, h, k);
        if(hash_valid(test2, h, iter)) {
            if(hash_get_key(test2, h, iter) != k || hash_get_val(test2, h, iter) != v) {
                ret = 4;
                goto DONE;
            }
            hash_erase(test2, h, iter);
        }
        else {
            ret = 5;
            goto DONE;
        }
    }

    if(!hash_empty(test2, h)) {
        ret = 3;
        goto DONE;
    }

DONE:
    hash_clear(test2, h);
    hash_destroy(test2, h);
    return ret;
}

static int test3(int number)
{
    int ret = 0;
    uint64_t k;
    int v, i;
    hash_t(test3) *h = hash_init(test3);
    if(!h) {
        return 1;
    }

    for(i = 0; i < number; i++) {
        k = (uint64_t) i;
        v = i << 1;
        ret = hash_insert(test3, h, k ,v);
        if(ret < 0)
            goto DONE;
    }

    if(hash_size(test3, h) != (uint32_t)number) {
        ret = 2;
        goto DONE;
    }

    for(i = 0; i < number; i++) {
        k = (uint64_t) i;
        v = i << 1;
        hash_iter_t iter = hash_find(test3, h, k);
        if(hash_valid(test3, h, iter)) {
            if(hash_get_key(test3, h, iter) != k || hash_get_val(test3, h, iter) != v) {
                ret = 4;
                goto DONE;
            }
            hash_erase(test3, h, iter);
        }
        else {
            ret = 5;
            goto DONE;
        }
    }

    if(!hash_empty(test3, h)) {
        ret = 3;
        goto DONE;
    }

DONE:
    hash_clear(test3, h);
    hash_destroy(test3, h);
    return ret;
}

char **strs;
const char *candidates = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const size_t total_len = 62;

static int str_generator_init(int number)
{
    strs = calloc(number, sizeof(char *));
    if(!strs)
        return -1;
    return 0;
}

static void str_generator_destroy(int number)
{
    for(int i = 0; i < number; i++)
        free(strs[i]);
    free(strs);
}

static char *str_generate()
{
    static uint32_t index = 0;
    char *ret = NULL;

    ret = malloc(16);
    if(!ret)
        return NULL;

    for(int i = 0; i < 15; i++)
        ret[i] = candidates[rand() % total_len];
    ret[15] = '\0';

    strs[index++] = ret;
    return ret;
}

static int test4(int number)
{
    int ret = 0;
    char *k;
    int v, i;
    hash_t(test4) *h = hash_init(test4);
    if(!h)
        return 1;

    if(str_generator_init(number))
        return 7;

    for(i = 0; i < number; i++) {
        k = str_generate();
        if(!k) {
            ret = 6;
            goto DONE;
        }
        v = i << 1;
        ret = hash_insert(test4, h, k ,v);
        if(ret < 0)
            goto DONE;
    }

    if(hash_size(test4, h) != (uint32_t)number) {
        ret = 2;
        goto DONE;
    }

    for(i = 0; i < number; i++) {
        k = strs[i];
        v = i << 1;
        hash_iter_t iter = hash_find(test4, h, k);
        if(hash_valid(test4, h, iter)) {
            if(hash_get_key(test4, h, iter) != k || hash_get_val(test4, h, iter) != v) {
                ret = 4;
                goto DONE;
            }
            hash_erase(test4, h, iter);
        }
        else {
            ret = 5;
            goto DONE;
        }
    }

    if(!hash_empty(test4, h)) {
        ret = 3;
        goto DONE;
    }

DONE:
    str_generator_destroy(number);
    hash_clear(test4, h);
    hash_destroy(test4, h);
    return ret;
}

static inline void print_usage(int code)
{
    puts("Usage: hash_test -n <NUMBER>   default: 1000000");
    exit(code);
}

int main(int argc, char *argv[])
{
    int c, number = 1000000;
    int ret = 0, err = 0;

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
        fprintf(stderr, "nagetive entry NUMBER: %d\n", number);
        exit(1);
    }

    ret = test1(number);
    if(ret) {
        fprintf(stderr, "test1 failed, errno=%d\n", ret);
        err = 1;
    }

    ret = test2(number);
    if(ret) {
        fprintf(stderr, "test2 failed, errno=%d\n", ret);
        err = 1;
    }

    ret = test3(number);
    if(ret) {
        fprintf(stderr, "test3 failed, errno=%d\n", ret);
        err = 1;
    }

    ret = test4(number);
    if(ret) {
        fprintf(stderr, "test4 failed, errno=%d\n", ret);
        err = 1;
    }

    return err;
}
