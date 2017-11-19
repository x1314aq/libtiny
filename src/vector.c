//
// Created by xaq on 11/19/17.
//

#include <string.h>
#include "vector.h"

/* -------------------------- private prototypes ---------------------------- */
void _expand(vector *v);

/* ----------------------------- API implementation ------------------------- */
vector *vector_init(int n, size_t ele_size){
    vector *_v = (vector *) malloc(sizeof(vector));
    _v->ele_size = ele_size;
    _v->start = malloc(n * ele_size);
    _v->finish = _v->start;
    _v->end_of_storage = _v->start + (n * ele_size);
    _v->value_free = NULL;
    return _v;
}

size_t vector_size(vector *v){
    return (size_t) ((v->finish - v->start) / v->ele_size);
}

size_t vector_capacity(vector *v){
    return (size_t) ((v->end_of_storage - v->start) / v->ele_size);
}

void *vector_at(vector *v, size_t position){
#if defined(_DEBUG) || defined(DEBUG)
    if(position >= vector_size(v)){
        puts("out of range!.");
        printf("size: %d, position: %d", vector_size(v), position);
        return NULL;
    }
#endif
    return v->start + position * v->ele_size;
}

void *vector_back(vector *v){
    return v->finish - v->ele_size;
}

void *vector_front(vector *v){
    return v->start;
}

void vector_push_back(vector *v, void *element){
    if(v->finish == v->end_of_storage)
        _expand(v);

    register char *des = (char *)v->finish;
    register char *src = (char *)element;
    for(size_t i = 0; i < v->ele_size; i++)
        *des++ = *src++;
    v->finish = des;
}

void vector_pop_back(vector *v){
    v->finish -= v->ele_size;
    if(v->value_free)
        v->value_free(v->finish);
}

void vector_insert(vector *v, size_t position, void *element){
    if(v->finish == v->end_of_storage)
        _expand(v);

    void *final = vector_at(v, position);
    if(final > v->finish) return;
    else if(final == v->finish) vector_push_back(v, element);
    else{
        /* 整体移动后继元素 */
        register char *des = (char *)(v->finish + v->ele_size);
        register char *src = (char *)v->finish;
        void *ret = (void *)des;
        while(src != final)
            *des-- = *src--;

        /* 插入新元素 */
        des = final;
        src = element;
        for(size_t i = 0; i < v->ele_size; i++)
            *des++ = *src++;

        v->finish = ret;
    }
}

void vector_erase(vector *v, size_t position){
//    void *final = vector_at(v, position);
//    if(final >= v->finish) return;
//    if(v->value_free) v->value_free(final);
//
//    register char *src = (char *)(final + v->ele_size);
//    register char *des = (char *)final;
//
//    while(src != v->finish) *des++ = *src++;
//    v->finish  = des;
    vector_erase_range(v, position, position + 1);
}

void vector_erase_range(vector *v, size_t first, size_t last){
    if(first >= last) return;
    void *final = vector_at(v, last);
    if(final > v->finish) return;

    void *start = vector_at(v, first);
    register char *des = (char *)start;
    register char *src = (char *)final;

    while(final != v->finish){
        if(v->value_free)
            v->value_free(des);
        for(size_t i = 0; i < v->ele_size; i++)
            *des++ = *src++;
    }
    v->finish = des;
}

void vector_free(vector *v){
    void *tmp = v->start;
    if(v->value_free)
        for(; tmp != v->finish; tmp += v->ele_size)
            v->value_free(tmp);

    free(v);
}

/* ------------------------ private API implementation ---------------------- */
void _expand(vector *v){
    void *old = v->start;
    size_t cap = vector_capacity(v);
    v->start = malloc((size_t)(1.5 * cap) * v->ele_size);
    memcpy(v->start, old, v->finish - old);
    if(v->value_free)
        for(void *start = old; start != v->finish; start += v->ele_size)
            v->value_free(start);
    free(old);
    v->finish = v->start + cap * v->ele_size;
    v->end_of_storage = v->start + (size_t)(1.5 * cap) * v->ele_size;
}