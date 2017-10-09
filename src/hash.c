//
//  hash.c
//  my_stl
//
//  Created by 叶鑫 on 2017/7/25.
//  Copyright © 2017年 xaq. All rights reserved.
//

#include "hash.h"
#include <string.h>


#define min(x, y) ((x) < (y) ? (x) : (y))
/* 返回[c, n)之间不小于c的最小素数 */
int primeNLT(int c, int n){
    bitmap *bm = bitmap_init(n);
    bitmap_set(bm, 0);
    bitmap_set(bm, 1);
    for(int i = 2; i < n; i++){
        if(!bitmap_test(bm, i)){
            for(int j = min(i, 46340) * min(i, 46340); j < n; j += i){
                bitmap_set(bm, j);
            }
        }
    }
    while(c < n){
        if(bitmap_test(bm, c)) c++;
        else break;
    }
    bitmap_free(bm);
    return c;
}

hashmap *hash_init(int maxsz){
    hashmap *hm = (hashmap *)malloc(sizeof(hashmap));
    hm->_bucket = primeNLT(maxsz, 1048576);   //不小于maxsz的最小素数
    hm->_size = 0;
    hm->_cont = (hashnode **)calloc(hm->_bucket, sizeof(hashnode *));
    hm->_lr = bitmap_init(hm->_bucket);
    return hm;
}

void hash_free(hashmap *hm){
    for(int i = 0; i < hm->_bucket; i++){
        free(hm->_cont[i]);
    }
    free(hm->_cont);
    bitmap_free(hm->_lr);
    hm->_cont = NULL;
    hm->_lr = NULL;
    free(hm);
}

void hash_put(hashmap *hm, int key, void *val){
    if(2 * hm->_size > hm->_bucket) return;  //需要扩容和rehash，暂时不考虑
    int hash = abs(key) % hm->_bucket;
    hashnode *node;
    while((node = hm->_cont[hash])){
        hash = (hash + 1) % hm->_bucket;
    }
    node = (hashnode *)malloc(sizeof(hashnode));
    node->key = key;
    node->val = val;
    hm->_cont[hash] = node;
    hm->_size++;
}

hashnode *hash_get(hashmap *hm, int key){
    int hash = abs(key) % hm->_bucket;
    hashnode *node;
    while((node = hm->_cont[hash]) || bitmap_test(hm->_lr, hash)){
        if(node && node->key == key) return node;
        hash = (hash + 1) % hm->_bucket;
    }
    return NULL;
}

void hash_rm(hashmap *hm, int key){
    int hash = abs(key) % hm->_bucket;
    hashnode *node;
    while((node = hm->_cont[hash]) || bitmap_test(hm->_lr, hash)){
        if(node && node->key == key){
            free(node);
            node = NULL;
            bitmap_set(hm->_lr, hash);
            hm->_size--;
            return;
        }
        hash = (hash + 1) % hm->_bucket;
    }
}
