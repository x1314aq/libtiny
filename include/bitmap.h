//
//  Created by x1314aq on 2017/7/25.
//

#ifndef _LIBTINY_BITMAP_H_
#define _LIBTINY_BITMAP_H_

#include "common.h"


typedef struct{
    int n;
    char *m;
} bitmap;

BEGIN_DECL

/* 初始化bitmap，默认所有bit都是0 */
bitmap *bitmap_init(int n);

/* 将第k位的bit设置为1 */
void bitmap_set(bitmap *bm, int k);

/* 将第k位第bit设置为0 */
void bitmap_clear(bitmap *bm, int k);

/* 输出第k位bit的值 */
int bitmap_test(bitmap *bm, int k);

/* 翻转第k位bit的值(0->1, 1->0) */
void bitmap_reverse(bitmap *bm, int k);

/* 释放所用的空间 */
void bitmap_free(bitmap *bm);

END_DECL

#endif // _LIBTINY_BITMAP_H_
