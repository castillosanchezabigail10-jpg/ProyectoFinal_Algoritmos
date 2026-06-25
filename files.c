#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client_bst.h"
#include "files.h"

#define CLIENTS_FILE "clients.txt"
//Probando
/*  Formato del archivo clients.txt
   Una línea por cliente, campos separados por '|':
   id|nombre|telefono|correo|direccion
   Ejemplo:
   1001|Ana Rojas|8888-1111|ana@mail.com|San Jose Centro
   */

/* Cargar clientes desde archivo */
/* entrada:  root — raíz actual del BST (puede ser NULL al inicio)
   salida: raíz del BST con todos los clientes del archivo cargados
   restricciones:  si el archivo no existe, retorna root sin cambios */
BSTNode* load_clients(BSTNode *root) {
    FILE *fp = fopen(CLIENTS_FILE, "r");
    if (fp == NULL) {
        printf("Nota: no se encontro %s. Se inicia sin clientes.\n", CLIENTS_FILE);
        return root;
    }

    Client c;
    char line[512];
    int loaded = 0;

    while (fgets(line, sizeof(line), fp)) {
        /* Eliminar salto de línea al final */
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0) continue;

        /* Parsear campos separados por '|' */
        char *token = strtok(line, "|");
        if (!token) continue;
        c.id = atoi(token);

        token = strtok(NULL, "|"); if (!token) continue;
        strncpy(c.name, token, MAX_STR - 1);

        token = strtok(NULL, "|"); if (!token) continue;
        strncpy(c.phone, token, MAX_STR - 1);

        token = strtok(NULL, "|"); if (!token) continue;
        strncpy(c.email, token, MAX_STR - 1);

        token = strtok(NULL, "|"); if (!token) continue;
        strncpy(c.address, token, MAX_STR - 1);

        root = bst_insert(root, c);
        loaded++;
    }

    fclose(fp);
    printf("Clientes cargados desde archivo: %d\n", loaded);
    return root;
}

/*  Guardar clientes en archivo (usar en Día 3)  */
/* Input:  root  raíz del BST con todos los clientes
   Notas:  recorre inorden para guardar ordenados por ID */
static void save_node(FILE *fp, BSTNode *root) {
    if (root == NULL) return;
    save_node(fp, root->left);
    fprintf(fp, "%d|%s|%s|%s|%s\n",
            root->data.id,
            root->data.name,
            root->data.phone,
            root->data.email,
            root->data.address);
    save_node(fp, root->right);
}

void save_clients(BSTNode *root) {
    FILE *fp = fopen(CLIENTS_FILE, "w");
    if (fp == NULL) {
        printf("Error: no se pudo abrir %s para escritura.\n", CLIENTS_FILE);
        return;
    }
    save_node(fp, root);
    fclose(fp);
    printf("Clientes guardados en %s.\n", CLIENTS_FILE);
}
