#ifndef PACKAGE_AVL_H
#define PACKAGE_AVL_H

#define MAX_STR 100

typedef struct Package {
    int    id;
    char   description[MAX_STR];
    char   status[MAX_STR];
    int    client_id;
    char   destination[MAX_STR];
} Package;

typedef struct AVLNode {
    Package data;
    int     height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

AVLNode* avl_insert(AVLNode *root, Package p);
AVLNode* avl_search(AVLNode *root, int id);
void     avl_inorder(AVLNode *root);
void     avl_free(AVLNode *root);

#endif
