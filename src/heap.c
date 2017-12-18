//
//  heap.c
//  my_stl
//
//  Created by 叶鑫 on 2017/7/24.
//  Copyright © 2017年 xaq. All rights reserved.
//

#include "heap.h"


#define inheap(n, i)  (((-1) < (i)) && ((i) < (n)))
#define parent(i) ((i - 1) >> 1)
#define lastinternal(n) parent(n - 1)
#define lchild(i) (1 + ((i) << 1))
#define rchild(i) ((1 + (i)) << 1)
#define SWAP(base, a, b, size)               \
    do{                                      \
        size_t __size = (size);              \
        char *__a = (base) + __size * (a);   \
        char *__b = (base) + __size * (b);   \
        do{                                  \
            char __tmp = *__a;               \
            *__a++ = *__b;                   \
            *__b++ = __tmp;                  \
        } while(--__size > 0);               \
    } while(0)

void heap_percolate_up(heap *hp){
    int i = hp->_size - 1;
    while(i > 0){
        int p = parent(i);
        if((*hp->_cmp)(hp->_cont + i * hp->_ele_size, hp->_cont + p * hp->_ele_size) > 0) break;
        SWAP(hp->_cont, i, p, hp->_ele_size);
        i = p;
    }
}

void heap_percolate_down(heap *hp, int i){
    int j = lchild(i);
    while(j < hp->_size){
        if((j + 1) < hp->_size){
            if((*hp->_cmp)(hp->_cont + i * hp->_ele_size, hp->_cont + j * hp->_ele_size) >= 0){
                if((*hp->_cmp)(hp->_cont + i * hp->_ele_size, hp->_cont + (j + 1) * hp->_ele_size) >= 0) break;
                else {
                    SWAP(hp->_cont, i, j + 1, hp->_ele_size);
                    i = j + 1;
                }
            }
            else{
                if((*hp->_cmp)(hp->_cont + j * hp->_ele_size, hp->_cont + (j + 1) * hp->_ele_size) >= 0){
                    SWAP(hp->_cont, i, j, hp->_ele_size);
                    i = j;
                }
                else {
                    SWAP(hp->_cont, i, j + 1, hp->_ele_size);
                    i = j + 1;
                }
            }
        }
        else{
            if((*hp->_cmp)(hp->_cont + i * hp->_ele_size, hp->_cont + j * hp->_ele_size) >= 0) break;
            else {
                SWAP(hp->_cont, i, j, hp->_ele_size);
                i = j + 1;
            }
        }
        j = lchild(i);
    }
}

heap *heap_init(int maxsz, size_t ele_size, int (*cmp)(const void *, const void *)){
    heap *hp = malloc(sizeof(heap));
    hp->_size = 0;
    hp->_max = maxsz;
    hp->_ele_size = ele_size;
    hp->_cont = malloc(maxsz * hp->_ele_size);
    hp->_cmp = cmp;
    return hp;
}

heap *heap_heapify(void *base, int size, size_t ele_size, int (*cmp)(const void *, const void *)){
    heap *hp = heap_init(2 * size, ele_size, cmp);
    hp->_size = size;
    hp->_cont = base;
    for(int i = lastinternal(size); i > -1; i--){
        heap_percolate_down(hp, i);
    }
    return hp;
}

void heap_free(heap *hp){
    free(hp->_cont);
    hp->_cont = NULL;
}

void *heap_get_max(heap *hp){
    return hp->_cont;
}

void heap_insert(heap *hp, void *ele){
    if(hp->_size < hp->_max){
        size_t size = hp->_ele_size;
        register char *a = hp->_cont + size * hp->_size;
        register char *b = ele;
        do{
            *a++ = *b++;
        }while(--size > 0);
        hp->_size++;
        heap_percolate_up(hp);
    }
}

void heap_rm_max(heap *hp){
    if(hp->_size > 0) {
        hp->_size--;
        SWAP(hp->_cont, 0, hp->_size, hp->_ele_size);
        heap_percolate_down(hp, 0);
    }
}
