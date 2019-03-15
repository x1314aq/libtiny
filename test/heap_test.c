//
// Created by x1314aq on 2019/03/15.
//

#include "heap.h"


#define default_int32_lt_finc(a, b)    ((a) < (b))
HEAP_DECLARE(test1, int);
HEAP_IMPL(test1, int, default_int32_lt_finc);

static inline void print_usage(int code)
{
    puts("Usage: heap_test -n <NUMBER>  default: 1000000");
    exit(code);
}

static int cmp(const void *a, const void *b)
{
    return *(int *) a < *(int *) b ? -1 : 1;
}

int main(int argc, char *argv[])
{
    int c, number = 1000000;
    
    while((c = getopt(argc, argv, "hn:")) != -1) {
        switch(c) {
            case 'n':
                number = (int) strtol(optarg, NULL, 10);
                break;
            case 'h':
            case '?':
            default:
                print_usage(0);
        }
    }

    if(number < 0) {
        fprintf(stderr, "nagetive entry NUMBER: %d\n", number);
        exit(1);
    }

    int *arr1 = malloc(number * sizeof(int));
    int *arr2 = malloc(number * sizeof(int));
    int *arr3 = malloc(number * sizeof(int));
    for(int i = 0; i < number; i++)
        arr1[i] = rand();
    memcpy(arr3, arr1, number * sizeof(int));

    heap_t(test1) *h = heap_init(test1);
    for(int i = 0; i < number; i++)
        heap_push(test1, h, arr1[i]);

    for(int i = number - 1; i > -1; i--)
        arr2[i] = heap_pop(test1, h);

    qsort(arr1, number, sizeof(int), cmp);
    for(int i = 0; i < number; i++) {
        arr2[i] -= arr1[i];
        if(arr2[i] != 0)
            printf("error1 at %d\n", i);
    }

    int *end = (int *) ((char *) arr3 + number * sizeof(int));
    heap_make(test1, arr3, end);
    heap_t(test1) temp = {
        .num = number,
        .max = number,
        .room = arr3
    };
    heap_sort(test1, &temp);
    for(int i = 0; i < number; i++) {
        arr3[i] -= arr1[i];
        if(arr3[i] != 0)
            printf("error2 at %d\n", i);
    }

    free(arr1);
    free(arr2);
    free(arr3);
    heap_destroy(test1, h);
    return 0;
}
