//
// Created by x1314aq on 2019/03/23.
//

#include "sort.h"


#define default_lt_func(a, b)    ((a) < (b))
SORT_IMPL(test, int, default_lt_func);

static int cmp(const void *a, const void *b)
{
    return *(int *) a < *(int *) b ? -1 : 1;
}

static inline void print_usage(int code)
{
    puts("Usage: sort_test -n <NUMBER>  default: 1000000");
    exit(code);
}

int main(int argc, char *argv[])
{
    int c, number = 1000000;
    int err = 0;

    while((c = getopt(argc, argv, "hn:")) != -1) {
        switch(c) {
            default:
                print_usage(1);
            case '?':
            case 'h':
                print_usage(0);
            case 'n':
                number = (int) strtol(optarg, NULL, 10);
                break;
        }
    }

    if(number < 0) {
        fprintf(stderr, "nagetive entry NUMBER: %d\n", number);
        exit(1);
    }

    int *arr1 = malloc(number * sizeof(int));
    int *arr2 = malloc(number * sizeof(int));
    int *arr3 = malloc(number * sizeof(int));
    int *arr4 = malloc(number * sizeof(int));
    for(int i = 0; i < number; i++)
        arr1[i] = rand();
    memcpy(arr2, arr1, number * sizeof(int));
    memcpy(arr3, arr1, number * sizeof(int));
    memcpy(arr4, arr1, number * sizeof(int));

    qsort(arr1, number, sizeof(int), cmp);
    merge_sort(test, arr2, number);
    for(int i = 0; i < number; i++) {
        arr2[i] -= arr1[i];
        if(arr2[i] != 0) {
            printf("error in merge_sort at %d\n", i);
            err = 1;
        }
    }

    comb_sort(test, arr3, number);
    for(int i = 0; i < number; i++) {
        arr3[i] -= arr1[i];
        if(arr3[i] != 0) {
            printf("error in comb_sort at %d\n", i);
            err = 1;
        }
    }

    intro_sort(test, arr4, number);
    for(int i = 0; i < number; i++) {
        arr4[i] -= arr1[i];
        if(arr4[i] != 0) {
            printf("error in intro_sort at %d\n", i);
            err = 1;
        }
    }

    free(arr1);
    free(arr2);
    free(arr3);
    free(arr4);
    return err;
}
