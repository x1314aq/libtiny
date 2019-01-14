//
// Created by x1314aq on 11/19/17.
//

#ifndef _LIBTINY_VECTOR_H_
#define _LIBTINY_VECTOR_H_

#include "common.h"

typedef struct
{
    size_t ele_size;
    void *start;
    void *finish;
    void *end_of_storage;

    void (*value_free)(void *);
} vector;

BEGIN_DECL

/* Constructor */
vector *vector_init(int n, size_t ele_size);

/* Capacity */
size_t vector_size(vector *v);

size_t vector_capacity(vector *v);

/* Element access */
void *vector_at(vector *v, size_t position);

void *vector_back(vector *v);

void *vector_front(vector *v);

/* Modifiers */
void vector_push_back(vector *v, void *element);

void vector_pop_back(vector *v);

void vector_resize(vector *v, size_t new_size);

/* 清空当前vector，但是保持其capacity不变 */
void vector_clear(vector *v);

/* 表示在position前面插入元素 */
void vector_insert(vector *v, size_t position, void *element);

void vector_erase(vector *v, size_t position);

void vector_erase_range(vector *v, size_t first, size_t last);

/* destructor */
void vector_free(vector *v);

END_DECL

#endif //_LIBTINY_VECTOR_H_
