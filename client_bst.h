#ifndef CLIENT_BST_H
#define CLIENT_BST_H

#define MAX_STR 100

typedef struct Client {
    int id;
    char name[MAX_STR];
    char phone[MAX_STR];
    char email[MAX_STR];
    char address[MAX_STR];
} Client;

typedef struct BSTNode {
    Client data;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

BSTNode* bst_insert(BSTNode *root, Client c);
BSTNode* bst_search(BSTNode *root, int id);
void     bst_inorder(BSTNode *root);
void     bst_preorder(BSTNode *root);
void     bst_postorder(BSTNode *root);
void     bst_free(BSTNode *root);

#endif
