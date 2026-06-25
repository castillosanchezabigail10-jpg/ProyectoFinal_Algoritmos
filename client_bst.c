#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client_bst.h"
 //probando github
/* Crear nodo */
/* entrada:  Client c — datos del cliente a almacenar
   salida: puntero al nuevo nodo, o NULL si falla malloc */
static BSTNode* create_node(Client c) {
    BSTNode *node = (BSTNode*)malloc(sizeof(BSTNode));
    if (!node) {
        printf("Error: no se pudo asignar memoria para el nodo.\n");
        return NULL;
    }
    node->data  = c;
    node->left  = NULL;
    node->right = NULL;
    return node;
}

/* Insertar cliente */
/* entrada:  root — raíz actual del árbol (puede ser NULL)
           c    — cliente a insertar
   salida: nueva raíz del árbol
   restricciones :  rechaza IDs duplicados; usa recursión */
BSTNode* bst_insert(BSTNode *root, Client c) {
    if (root == NULL) {
        return create_node(c);
    }
    if (c.id < root->data.id) {
        root->left  = bst_insert(root->left, c);
    } else if (c.id > root->data.id) {
        root->right = bst_insert(root->right, c);
    } else {
        printf("Error: ya existe un cliente con ID %d.\n", c.id);
    }
    return root;
}

/*  Buscar por ID  */
/* entrada:  root — raíz del árbol
           id   — identificación a buscar
  salida : puntero al nodo encontrado, o NULL si no existe */
BSTNode* bst_search(BSTNode *root, int id) {
    if (root == NULL) {
        return NULL;
    }
    if (id == root->data.id) {
        return root;
    }
    if (id < root->data.id) {
        return bst_search(root->left, id);
    }
    return bst_search(root->right, id);
}

/*  Recorrido inorden (Izq → Nodo → Der)  */
/* salida: clientes impresos en orden ascendente de ID */
void bst_inorder(BSTNode *root) {
    if (root == NULL) return;
    bst_inorder(root->left);
    printf("  ID: %d | %s | %s | %s | %s\n",
           root->data.id,
           root->data.name,
           root->data.phone,
           root->data.email,
           root->data.address);
    bst_inorder(root->right);
}

/* ─ Recorrido preorden (Nodo → Izq → Der)  */
void bst_preorder(BSTNode *root) {
    if (root == NULL) return;
    printf("  ID: %d | %s\n", root->data.id, root->data.name);
    bst_preorder(root->left);
    bst_preorder(root->right);
}

/*  Recorrido postorden (Izq → Der → Nodo)  */
void bst_postorder(BSTNode *root) {
    if (root == NULL) return;
    bst_postorder(root->left);
    bst_postorder(root->right);
    printf("  ID: %d | %s\n", root->data.id, root->data.name);
}

/* Liberar memoria  */
/* Recorre en postorden para liberar hijos antes que el padre */
void bst_free(BSTNode *root) {
    if (root == NULL) return;
    bst_free(root->left);
    bst_free(root->right);
    free(root);
}
