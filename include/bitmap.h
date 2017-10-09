//
//  bitmap.h
//  my_stl
//
//  Created by 叶鑫 on 2017/7/25.
//  Copyright © 2017年 xaq. All rights reserved.
//

#ifndef bitmap_h
#define bitmap_h

#include <stdlib.h>


typedef struct{
    int n;
    char *m;
} bitmap;

bitmap *bitmap_init(int n);

void bitmap_set(bitmap *bm, int k);

void bitmap_clear(bitmap *bm, int k);

int bitmap_test(bitmap *bm, int k);

void bitmap_free(bitmap *bm);
#endif /* bitmap_h */
