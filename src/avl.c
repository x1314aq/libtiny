//
//  Created by x1314aq on 2017/7/22.
//

#include "avl.h"

#define depth(x) ((x) ? (x)->depth : 0)
#define balanced(x) (depth(x->left) == depth(x->right))
#define balfac(x) (depth(x->left) - depth(x->right))
#define avlbalanced(x) ((-2 < balfac(x)) && (balfac(x) < 2))


void avl_insert(TreeNode **root, int ele){
    TreeNode *hot;
    TreeNode **x = bt_search(root, ele, &hot);
    if(*x) return;
    *x = tn_init(ele);
    (*x)->parnet = hot;
    bt_update_depth(*root);
    for(TreeNode *g = hot; g; g = g->parnet){
        if(!avlbalanced(g)){
            TreeNode *p, *v;
            if(depth(g->left) > depth(g->right)) p = g->left;
            else if(depth(g->left) < depth(g->right)) p = g->right;
            else if(g->parnet && g->parnet->left == g) p = g->left;
            else p = g->right;
            if(depth(p->left) > depth(p->right)) v = p->left;
            else if(depth(p->left) < depth(p->right)) v = p->right;
            else if(p->parnet && p->parnet->left == p) v = p->left;
            else v = p->right;
            if(g == *root){
                *root = bt_rotate(g, p, v);
            }
            else if(g->parnet->left == g){
                TreeNode *temp = g->parnet;
                temp->left = bt_rotate(g, p, v);
            }
            else{
                TreeNode *temp = g->parnet;
                temp->right = bt_rotate(g, p, v);
            }
            break;
        }
    }
}

void avl_remove(TreeNode **root, int ele){
    TreeNode *hot;
    TreeNode **x = bt_search(root, ele, &hot);
    TreeNode *w = *x;
    TreeNode *succ = NULL;
    if(!w) return;
    if(!w->left) succ = (*x) = (*x)->right;
    else if(!w->right) succ = (*x) = (*x)->left;
    else{
        w = tn_succ(w);
        int temp = (*x)->val;
        (*x)->val = w->val;
        w->val = temp;
        TreeNode *u = w->parnet;
        if(u == (*x)) u->right = succ = w->right;
        else u->left = succ = w->right;
    }
    hot = w->parnet;
    if(succ) succ->parnet = hot;
    free(w);
    w = NULL;
    int size = bt_update_depth(*root);
    if(!size) return;
    for(TreeNode *g = hot; g; g = g->parnet){
        if(!avlbalanced(g)){
            TreeNode *p, *v;
            if(depth(g->left) > depth(g->right)) p = g->left;
            else if(depth(g->left) < depth(g->right)) p = g->right;
            else if(g->parnet && g->parnet->left == g) p = g->left;
            else p = g->right;
            if(depth(p->left) > depth(p->right)) v = p->left;
            else if(depth(p->left) < depth(p->right)) v = p->right;
            else if(p->parnet && p->parnet->left == p) v = p->left;
            else v = p->right;
            if(g == *root){
                *root = bt_rotate(g, p, v);
            }
            else if(g->parnet->left == g){
                TreeNode *temp = g->parnet;
                temp->left = bt_rotate(g, p, v);
            }
            else{
                TreeNode *temp = g->parnet;
                temp->right = bt_rotate(g, p, v);
            }
            bt_update_depth(*root);
        }
    }
}
