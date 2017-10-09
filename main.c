//
// Created by xaq on 10/9/17.
//

#include "map.h"
#include <stdlib.h>
#include "algorithm.h"
#include <time.h>
#include <stdio.h>


uint64_t hash_func(const void *key){
    return _default_int_hash_func(*(uint32_t *)key);
}

int* twoSum(int* nums, int numsSize, int target) {
    map_type *type = malloc(sizeof(map_type));
    type->value_dup = NULL;
    type->value_free = NULL;
    type->hash_func = hash_func;
    map *m = map_create(type);
    int res;
    for(int i = 0; i < numsSize; i++){
        res = target - nums[i];
        map_entry *entry = map_find(m, res);
        if(entry){
            int *result = malloc(2 * sizeof(int));
            result[0] = i;
            result[1] = entry->v.s64;
            return result;
        }
        else map_put(m, nums[i], (void *)i);
    }
    map_free(m);
    return NULL;
}

int cmp(const void *a, const void *b){
    int _a = *(int *)(a);
    int _b = *(int *)(b);
    if(_a < _b) return -1;
    else if(_a == _b) return 0;
    else return 1;
}

int main(){
    const int size = 50000;
    int *nums = malloc(size * sizeof(int));
    srand(time(NULL));
    for(int i = 0; i < size; i++){
        nums[i] = rand();
    }
    clock_t start = clock();
    quick_sort(nums, 0, size, sizeof(int), cmp);
//    qsort(nums, size, sizeof(int), cmp);
    clock_t end = clock();
    printf("time: %lfms\n", (double)(end - start) / CLOCKS_PER_SEC * 1000.0);
    free(nums);
    return 0;
}
