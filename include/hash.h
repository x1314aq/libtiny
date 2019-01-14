//
//  Created by x1314aq on 2017/7/25.
//

#ifndef _LIBTINY_HASH_H_
#define _LIBTINY_HASH_H_

#include "common.h"
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

BEGIN_DECL

hashmap *hash_init(int maxsz);

void hash_free(hashmap *hm);

void hash_put(hashmap *hm, int key, void *val);

hashnode *hash_get(hashmap *hm, int key);

void hash_rm(hashmap *hm, int key);

END_DECL

#endif // _LIBTINY_HASH_H_
