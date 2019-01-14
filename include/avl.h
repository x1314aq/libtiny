//
//  Created by x1314aq on 2017/7/22.
//

#ifndef _LIBTINY_AVL_H_
#define _LIBTINY_AVL_H_

#include "bintree.h"
#include "common.h"

BEGIN_DECL

void avl_insert(TreeNode **root, int ele);

void avl_remove(TreeNode **root, int ele);

END_DECL

#endif //_LIBTINY_AVL_H_
