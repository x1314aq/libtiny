//
//  bintree.h
//  my_stl
//
//  Created by 叶鑫 on 2017/7/22.
//  Copyright © 2017年 xaq. All rights reserved.
//

#ifndef bintree_h
#define bintree_h

#include <stdlib.h>


#define max(x, y) ((x) > (y) ? (x) : (y))


typedef enum{
    RB_RED,
    RB_BLACK
} RBColor;

struct TreeNode{
    int val;
    struct TreeNode *parnet;
    struct TreeNode *left;
    struct TreeNode *right;
    int depth;
    RBColor color;
};

typedef struct TreeNode TreeNode;

#ifdef __cplusplus
extern "C" {
#endif

TreeNode *tn_init(int ele);

void tn_insert_as_lc(TreeNode *root, int ele);

void tn_insert_as_rc(TreeNode *root, int ele);

TreeNode *tn_succ(TreeNode *root);

int bt_update_depth(TreeNode *root);

TreeNode *bt_connect34(TreeNode *a, TreeNode *b, TreeNode *c, TreeNode *T0, TreeNode *T1, TreeNode *T2, TreeNode *T3);

TreeNode *bt_rotate(TreeNode *g, TreeNode *p, TreeNode *v);

TreeNode **bt_search(TreeNode **root, int ele, TreeNode **hot);

void bt_insert(TreeNode **root, int ele);

void bt_remove(TreeNode **root, int ele);

void bt_free(TreeNode *root);

#ifdef __cplusplus
}
#endif

#endif /* bintree_h */
