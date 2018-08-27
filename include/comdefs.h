//
// Created by 叶鑫 on 2018/8/27.
//

#ifndef STL_COMDEFS_H
#define STL_COMDEFS_H


#ifndef offsetof
#define offsetof(type, member)  ((size_t) &((type *)0)->member)

#ifndef container_of
#define container_of(ptr, type, member) ({    \
        const typeof(((type *)0)->member)* __mptr = (ptr);    \
        (type *)((char *)__mptr - offsetof(type, member));})

#endif //STL_COMDEFS_H
