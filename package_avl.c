#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "package_avl.h"

/* FUNCIONES AUXILIARES DE ALTURA Y BALANCE */

/* entrada:  nodo (puede ser NULL)
   salida: altura almacenada, o 0 si el nodo es NULL */
static int height(AVLNode *n) {
    return n ? n->height : 0;
}

/* entrada:  dos enteros
   salida: el mayor de los dos */
static int max2(int a, int b) {
    return a > b ? a : b;
}

/* Recalcula la altura de un nodo a partir de sus hijos */
static void update_height(AVLNode *n) {
    n->height = 1 + max2(height(n->left), height(n->right));
}

/* entrada:  nodo
   salida: factor de balance = altura(izq) - altura(der)
           >1 → cargado a la izquierda
           <-1 → cargado a la derecha */
static int balance_factor(AVLNode *n) {
    return n ? height(n->left) - height(n->right) : 0;
}

/* ROTACIONES */

/*  Rotación simple derecha (caso izquierda-izquierda)
    Antes:       y            Después:    x
                / \                      / \
               x   C                   A   y
              / \                         / \
             A   B                       B   C
*/
static AVLNode* rotate_right(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *B = x->right;

    x->right = y;
    y->left  = B;

    update_height(y);
    update_height(x);
    return x;
}

/*  Rotación simple izquierda (caso derecha-derecha)
    Antes:    x              Después:      y
             / \                          / \
            A   y                        x   C
               / \                      / \
              B   C                    A   B
*/
static AVLNode* rotate_left(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *B = y->left;

    y->left  = x;
    x->right = B;

    update_height(x);
    update_height(y);
    return y;
}

/* CREAR NODO  */

/* entrada:  Package p — datos del paquete
   salida: nuevo nodo con altura 1, hijos NULL */
static AVLNode* create_node(Package p) {
    AVLNode *node = (AVLNode*)malloc(sizeof(AVLNode));
    if (!node) {
        printf("Error: no se pudo asignar memoria.\n");
        return NULL;
    }
    node->data   = p;
    node->height = 1;
    node->left   = NULL;
    node->right  = NULL;
    return node;
}

/* INSERCIÓN CON REBALANCEO */

/* entrada:  root — raíz actual (puede ser NULL)
           p    — paquete a insertar
   salida: nueva raíz del subárbol (puede cambiar por rotación)
   Lógica:
     1. Insertar como BST normal
     2. Actualizar altura
     3. Calcular factor de balance
     4. Aplicar rotación si |balance| > 1                          */
AVLNode* avl_insert(AVLNode *root, Package p) {
    /* Paso 1: inserción BST */
    if (root == NULL) return create_node(p);

    if (p.id < root->data.id) {
        root->left  = avl_insert(root->left, p);
    } else if (p.id > root->data.id) {
        root->right = avl_insert(root->right, p);
    } else {
        printf("Error: ya existe un paquete con ID %d.\n", p.id);
        return root;
    }

    /* Paso 2: actualizar altura */
    update_height(root);

    /* Paso 3: factor de balance */
    int bf = balance_factor(root);

    /* Paso 4: cuatro casos de desbalance */

    /* Caso izquierda-izquierda: nuevo nodo en subárbol izq-izq
       Solución: rotación simple derecha                           */
    if (bf > 1 && p.id < root->left->data.id) {
        return rotate_right(root);
    }

    /* Caso derecha-derecha: nuevo nodo en subárbol der-der
       Solución: rotación simple izquierda                         */
    if (bf < -1 && p.id > root->right->data.id) {
        return rotate_left(root);
    }

    /* Caso izquierda-derecha: nuevo nodo en subárbol izq-der
       Solución: rotar hijo izq a la izquierda, luego raíz derecha */
    if (bf > 1 && p.id > root->left->data.id) {
        root->left = rotate_left(root->left);
        return rotate_right(root);
    }

    /* Caso derecha-izquierda: nuevo nodo en subárbol der-izq
       Solución: rotar hijo der a la derecha, luego raíz izquierda */
    if (bf < -1 && p.id < root->right->data.id) {
        root->right = rotate_right(root->right);
        return rotate_left(root);
    }

    return root;
}

/*  BÚSQUEDA */

/* entrada:  root — raíz del AVL
           id   — ID del paquete a buscar
   salida: puntero al nodo, o NULL si no existe */
AVLNode* avl_search(AVLNode *root, int id) {
    if (root == NULL)        return NULL;
    if (id == root->data.id) return root;
    if (id < root->data.id)  return avl_search(root->left, id);
    return avl_search(root->right, id);
}

/* RECORRIDO INORDEN  */

/* Imprime paquetes en orden ascendente de ID */
void avl_inorder(AVLNode *root) {
    if (root == NULL) return;
    avl_inorder(root->left);
    printf("  ID: %d | %s | Estado: %s | Cliente: %d | Destino: %s\n",
           root->data.id,
           root->data.description,
           root->data.status,
           root->data.client_id,
           root->data.destination);
    avl_inorder(root->right);
}

/* LIBERAR MEMORIA */

void avl_free(AVLNode *root) {
    if (root == NULL) return;
    avl_free(root->left);
    avl_free(root->right);
    free(root);
}
