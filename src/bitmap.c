//
//  bitmap.c
//  my_stl
//
//  Created by 叶鑫 on 2017/7/25.
//  Copyright © 2017年 xaq. All rights reserved.
//

#include "bitmap.h"


bitmap *bitmap_init(int n){
    bitmap *bm = (bitmap *)malloc(sizeof(bitmap));
    bm->n = (n + 7) / 8;
    bm->m = (char *)calloc(n, sizeof(char));
    return bm;
}

void bitmap_free(bitmap *bm){
    free(bm->m);
    bm->m = NULL;
    free(bm);
}

void bitmap_set(bitmap *bm, int k){
    if(k > 8 * bm->n) return;
    bm->m[k >> 3] |= (0x80 >> (k & 0x07));
}

void bitmap_clear(bitmap *bm, int k){
    if(k > 8 * bm->n) return;
    bm->m[k >> 3] &= (~(0x80 >> (k & 0x07)));
}

int bitmap_test(bitmap *bm, int k){
    if(k > 8 * bm->n) return -1;
    return bm->m[k >> 3] & (0x80 >> (k & 0x07));
}

void bitmap_reverse(bitmap *bm, int k){
    if(k > 8 * bm->n) return;
    bm->m[k >> 3] ^= (0x80 >> (k & 0x07));
}
