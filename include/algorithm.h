//
//  algorithm.h
//  my_stl
//
//  Created by 叶鑫 on 2017/7/26.
//  Copyright © 2017年 xaq. All rights reserved.
//

#ifndef algorithm_h
#define algorithm_h

#include <stdlib.h>


/* 蛮力串匹配算法 */
int bf_match(const char *t, const char *p);

/* KMP串匹配算法(基于next表) */
int kmp_match(const char *t, const char *p);

/* BM串匹配算法(基于坏字符策略和好后缀策略) */
int bm_match(const char *t, const char *p);

/* Karp-Rabin串匹配算法(基于hash表) */
int kr_match(const char *t, const char *p);

/* 快速排序算法 */
void quick_sort(void *base, int low, int high, size_t ele_size, int (*cmp)(const void *, const void *));

/* 选择排序算法 */
void selection_sort(void *base, int low, int high, size_t ele_size, int (*cmp)(const void *, const void *));

/* 插入排序算法 */
void insertion_sort(void *base, int low, int high, size_t ele_size, int (*cmp)(const void *, const void *));

/* 归并排序算法 */
void merge_sort(void *base, int low, int high, size_t ele_size, int (*cmp)(const void *, const void *));

/* 堆排序算法 */
void heap_sort(void *base, int low, int high, size_t ele_size, int (*cmp)(const void *, const void *));

#endif /* algorithm_h */
