//
// Created by x1314aq on 2019/03/25.
//


#include <algorithm>
#include "sort.h"

#define default_lt_func(a, b)    ((a) < (b))
SORT_IMPL(test, int, default_lt_func);

static int cmp(const void *a, const void *b)
{
    return *(int *) a < *(int *) b ? -1 : 1;
}

int main(int argc, char *argv[])
{
    int number = 5000000;
    uint64_t start, end;

    if(argc > 1)
        number = (int) strtol(argv[1], NULL, 10);

    int *a1 = (int *) malloc(number * sizeof(int));
    int *a2 = (int *) malloc(number * sizeof(int));
    int *a3 = (int *) malloc(number * sizeof(int));
    int *a4 = (int *) malloc(number * sizeof(int));
    int *a5 = (int *) malloc(number * sizeof(int));

    srand(time(NULL));
    for(int i = 0; i < number; i++)
        a1[i] = rand();
    memcpy(a2, a1, number * sizeof(int));
    memcpy(a3, a1, number * sizeof(int));
    memcpy(a4, a1, number * sizeof(int));
    memcpy(a5, a1, number * sizeof(int));

    start = stl_rdtsc();
    qsort(a1, number, sizeof(int), cmp);
    end = stl_rdtsc();
    printf("qsort: %llu\n", end - start);

    start = stl_rdtsc();
    std::sort(a2, a2 + number);
    end = stl_rdtsc();
    printf("std::sort: %llu\n", end - start);

    start = stl_rdtsc();
    merge_sort(test, a3, number);
    end = stl_rdtsc();
    printf("merge_sort: %llu\n", end - start);

    start = stl_rdtsc();
    intro_sort(test, a4, number);
    end = stl_rdtsc();
    printf("intro_sort: %llu\n", end - start);

    start = stl_rdtsc();
    comb_sort(test, a5, number);
    end = stl_rdtsc();
    printf("comb_sort: %llu\n", end - start);

    if(memcmp(a2, a1, sizeof(int) * number))
        printf("error a2 != a1\n");
    if(memcmp(a3, a1, sizeof(int) * number))
        printf("error a3 != a1\n");
    if(memcmp(a4, a1, sizeof(int) * number))
        printf("error a4 != a1\n");
    if(memcmp(a5, a1, sizeof(int) * number))
        printf("error a5 != a1\n");

    free(a1);
    free(a2);
    free(a3);
    free(a4);
    free(a5);
    return 0;
}
