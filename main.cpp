//
// Created by xaq on 10/10/17.
//

#include <iostream>
#include "algorithm.h"
#include <algorithm>
#include <cstring>
#include "stack.h"

int cmp(const void *a, const void *b) {
    int _a = *(int *) (a);
    int _b = *(int *) (b);
    if(_a < _b) return -1;
    else if(_a == _b) return 0;
    else return 1;
}

int main() {
//    int size;
//    std::cin >> size;
//    int *nums = new int[size];
//    int *nums2 = new int[size];
//
//    srand(time(NULL));
//    for(int i = 0; i < size; i++){
//        nums[i] = rand() % size;
//    }
//    memcpy(nums2, nums, size * sizeof(int));
//    quick_sort(nums, 0, size, sizeof(int), cmp);
//    std::sort(nums2, nums2 + size);
//
//    for(int i = 0; i < size; i++){
//        if(nums[i] != nums2[i])
//            std::cerr << "error" << std::endl;
//    }
//    delete[]nums;
//    delete[]nums2;
    stack *s = stack_init(sizeof(int));
    for(int i = 0; i < 1000; i++){
        int temp = rand();
        stack_push(s, &temp);
    }
    stack_free(s);
    return 0;
}