//
//  avl.h
//  my_stl
//
//  Created by 叶鑫 on 2017/7/22.
//  Copyright © 2017年 xaq. All rights reserved.
//

#ifndef avl_h
#define avl_h

#include "bintree.h"

#ifdef __cplusplus
extern "C" {
#endif

void avl_insert(TreeNode **root, int ele);

void avl_remove(TreeNode **root, int ele);

#ifdef __cplusplus
}
#endif

#endif /* avl_h */
