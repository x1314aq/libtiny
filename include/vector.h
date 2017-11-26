//
// Created by xaq on 11/19/17.
//

#ifndef TTQ_VECTOR_H
#define TTQ_VECTOR_H

#include <stdlib.h>

typedef struct
{
    size_t ele_size;
    void *start;
    void *finish;
    void *end_of_storage;

    void (*value_free)(void *);
} vector;

#ifdef __cplusplus
extern "C" {
#endif

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

/* 表示在position前面插入元素 */
void vector_insert(vector *v, size_t position, void *element);

void vector_erase(vector *v, size_t position);

void vector_erase_range(vector *v, size_t first, size_t last);

/* destructor */
void vector_free(vector *v);

#ifdef __cplusplus
}
#endif

#endif //TTQ_VECTOR_H
