//
//  Created by x1314aq on 2017/7/22.
//

#ifndef _LIBTINY_RBTREE_H_
#define _LIBTINY_RBTREE_H_

#include "bintree.h"

BEGIN_DECL

TreeNode *rb_init(int ele);

void rb_insert(TreeNode **root, int ele);

void rb_remove(TreeNode **root, int ele);

END_DECL

#endif // _LIBTINY_RBTREE_H_
