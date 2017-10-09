//
//  rbtree.c
//  my_stl
//
//  Created by 叶鑫 on 2017/7/22.
//  Copyright © 2017年 xaq. All rights reserved.
//

#include "rbtree.h"


#define isblack(x) (!(x) || (x)->color == RB_BLACK)
#define isred(x) (!isblack(x))
#define depth(x) ((x) ? (x)->depth : 0)


void rb_solve_double_red(TreeNode *v){
    if(!v->parnet){
        v->color = RB_BLACK;
//        bt_update_depth(v);
        return;
    }
    TreeNode *p = v->parnet;
    if(isblack(p)) return;
    TreeNode *g = p->parnet;
    TreeNode *u;
    if(g->left == p) u = g->right;
    else u = g->left;
    if(isblack(u)){
        TreeNode *temp = g->parnet;
        TreeNode *r = NULL;
        if(p == g->left){
            if(v == p->left) r = bt_connect34(v, p, g, v->left, v->right, p->right, g->right);
            else r = bt_connect34(p, v, g, p->left, v->left, v->right, g->right);
        }
        else{
            if(v == p->right) r = bt_connect34(g, p, v, g->left, p->left, v->left, v->right);
            else r = bt_connect34(g, v, p, g->left, v->left, v->right, p->right);
        }
        r->color = RB_BLACK;
        g->color = RB_RED;
        r->parnet = temp;
        if(temp){
            if(temp->left == g) temp->left = r;
            else temp->right = r;
        }
    }
    else{
        p->color = RB_BLACK;
        u->color = RB_BLACK;
        if(g->parnet) g->color = RB_RED;
        rb_solve_double_red(g);
    }
}

void rb_solve_double_black(TreeNode *r, TreeNode *hot){
    TreeNode *p = r ? r->parnet : hot;
    if(!p) return;
    TreeNode *s = (r == p->left) ? p->right : p->left;
    if(isblack(s)){
        TreeNode *t = NULL;
        if(isred(s->right)) t = s->right;
        if(isred(s->left)) t = s->left;
        if(t){
            RBColor oldcolor = p->color;
            TreeNode *temp = p->parnet;
            TreeNode *rotate = NULL;
            if(s == p->right){
                if(t == s->left) rotate = bt_connect34(p, t, s, r, t->left, t->right, s->right);
                else rotate = bt_connect34(p, s, t, r, s->left, t->left, t->right);
            }
            else{
                if(t == s->left) rotate = bt_connect34(t, s, p, t->left, t->right, s->right, r);
                else rotate = bt_connect34(s, t, p, s->left, t->left, t->right, r);
            }
            rotate->parnet = temp;
            if(temp->left == p) temp->left = rotate;
            else temp->right = rotate;
            if(rotate->left) rotate->left->color = RB_BLACK;
            if(rotate->right) rotate->right->color = RB_BLACK;
            rotate->color = oldcolor;
        }
        else{
            s->color = RB_RED;
            if(isred(p)) p->color = RB_BLACK;
            else rb_solve_double_black(p, hot);
        }
    }
    else{
        p->color = RB_RED;
        s->color = RB_BLACK;
        TreeNode *t = (s == p->left ? s->left : s->right);
        TreeNode *temp = p->parnet;
        TreeNode *rotate = NULL;
        if(s == p->left) rotate = bt_connect34(t, s, p, t->left, t->right, s->right, r);
        else rotate = bt_connect34(p, s, t, r, s->left, t->left, t->right);
        rotate->parnet = temp;
        if(temp->left == p) temp->left = rotate;
        else temp->right = rotate;
        hot = p;
        rb_solve_double_black(r, hot);
    }
}

TreeNode *rb_init(int ele){
    TreeNode *node = tn_init(ele);
    node->color = RB_BLACK;
    return node;
}

void rb_insert(TreeNode **root, int ele){
    TreeNode *hot;
    TreeNode **x = bt_search(root, ele, &hot);
    if(*x) return;
    (*x) = tn_init(ele);
    (*x)->parnet = hot;
    rb_solve_double_red(*x);
    while((*root)->parnet) *root = (*root)->parnet;
}

void rb_remove(TreeNode **root, int ele){
    TreeNode *hot;
    TreeNode **x = bt_search(root, ele, &hot);
    TreeNode *w = *x;
    TreeNode *succ = NULL;
    if(!w) return;
    if(!w->left) succ = (*x) = (*x)->right;
    else if(!w->right) succ = (*x) = (*x)->left;
    else{
        w = tn_succ(*x);
        int temp = (*x)->val;
        (*x)->val = w->val;
        w->val = temp;
        TreeNode *u = w->parnet;
        if(u == (*x)) u->right = succ = w->right;
        else u->left = succ = w->right;
    }
    hot = w->parnet;
    if(succ) succ->parnet = hot;
    RBColor color = w->color;
    free(w);
    w = NULL;
    int size = bt_update_depth(*root);
    if(!size) return;
    if(color == RB_RED) return;
    else{
        if(isred(succ)) succ->color = RB_BLACK;
        else {
            rb_solve_double_black(succ, hot);
            while ((*root)->parnet) *root = (*root)->parnet;
        }
    }
}
