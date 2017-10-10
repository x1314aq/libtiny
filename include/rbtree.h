//
//  rbtree.h
//  my_stl
//
//  Created by 叶鑫 on 2017/7/22.
//  Copyright © 2017年 xaq. All rights reserved.
//

#ifndef rbtree_h
#define rbtree_h

#include "bintree.h"

#ifdef __cplusplus
extern "C" {
#endif

TreeNode *rb_init(int ele);

void rb_insert(TreeNode **root, int ele);

void rb_remove(TreeNode **root, int ele);

#ifdef __cplusplus
}
#endif

#endif /* rbtree_h */
