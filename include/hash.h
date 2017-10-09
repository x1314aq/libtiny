//
//  hash.h
//  my_stl
//
//  Created by 叶鑫 on 2017/7/25.
//  Copyright © 2017年 xaq. All rights reserved.
//

#ifndef hash_h
#define hash_h

#include <stdlib.h>
#include "bitmap.h"

typedef struct{
    int key;
    void *val;
} hashnode;

typedef struct{
    int _bucket;       //总的桶的数目
    int _size;         //已经存放的词条的数目
    hashnode **_cont;  //实际存放词条的地方
    bitmap *_lr;       //懒惰删除标记（用bitmap实现）
} hashmap;

hashmap *hash_init(int maxsz);

void hash_free(hashmap *hm);

void hash_put(hashmap *hm, int key, void *val);

hashnode *hash_get(hashmap *hm, int key);

void hash_rm(hashmap *hm, int key);

#endif /* hash_h */
