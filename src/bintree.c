//
//  Created by x1314aq on 2017/7/22.
//

#include "bintree.h"


TreeNode *tn_init(int ele){
    TreeNode *node = malloc(sizeof(TreeNode));
    node->val = ele;
    node->parnet = NULL;
    node->left = NULL;
    node->right = NULL;
    node->depth = 1;
    node->color = RB_RED;
    return node;
}

void tn_insert_as_lc(TreeNode *root, int ele){
    root->left = tn_init(ele);
    root->left->parnet = root;
}

void tn_insert_as_rc(TreeNode *root, int ele){
    root->right = tn_init(ele);
    root->right->parnet = root;
}

TreeNode *tn_succ(TreeNode *root){
    TreeNode *temp = root;
    if(temp->right){
        temp = temp->right;
        while(temp->left) temp = temp->left;
    }
    else{
        while((temp->parnet) && (temp == temp->parnet->right)) temp = temp->parnet;
        temp = temp->parnet;
    }
    return temp;
}

int bt_update_depth(TreeNode *root){
    if(!root) return 0;
    int ldep = bt_update_depth(root->left);
    int rdep = bt_update_depth(root->right);
    int dep = MAX(ldep, rdep) + 1;
    root->depth = dep;
    return dep;
}

TreeNode *bt_connect34(TreeNode *a, TreeNode *b, TreeNode *c, TreeNode *T0, TreeNode *T1, TreeNode *T2, TreeNode *T3){
    a->left = T0;
    if(T0) T0->parnet = a;
    a->right = T1;
    if(T1) T1->parnet = a;
    c->left = T2;
    if(T2) T2->parnet = c;
    c->right = T3;
    if(T3) T3->parnet = c;
    b->left = a;
    b->right = c;
    a->parnet = b;
    c->parnet = b;
    bt_update_depth(b);
    return b;
}

TreeNode *bt_rotate(TreeNode *g, TreeNode *p, TreeNode *v){
    if(g->left == p){
        if(p->left == v){
            p->parnet = g->parnet;
            return bt_connect34(v, p, g, v->left, v->right, p->right, g->right);
        }
        else{
            v->parnet = g->parnet;
            return bt_connect34(p, v, g, p->left, v->left, v->right, g->right);
        }
    }
    else{
        if(p->right == v){
            p->parnet = g->parnet;
            return bt_connect34(g, p, v, g->left, p->left, v->left, v->right);
        }
        else{
            v->parnet = g->parnet;
            return bt_connect34(g, v, p, g->left, v->left, v->right, p->right);
        }
    }
}

TreeNode **bt_search(TreeNode **root, int ele, TreeNode **hot){
    if(!(*root) || ((*root)->val) == ele) return root;
    *hot = *root;
    return bt_search((ele < (*root)->val) ? &((*root)->left) : &((*root)->right), ele, hot);
}

void bt_insert(TreeNode **root, int ele){
    TreeNode *hot = NULL;
    TreeNode **x = bt_search(root, ele, &hot);
    if(*x) return;
    *x = tn_init(ele);
    (*x)->parnet = hot;
    bt_update_depth(*root);
}

void bt_remove(TreeNode **root, int ele){
    TreeNode *hot = NULL;
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
    bt_update_depth(*root);
}

void bt_free(TreeNode *root){
    if(!root) return;
    TreeNode *left = root->left;
    TreeNode *right = root->right;
    free(root);
    bt_free(left);
    bt_free(right);
}
