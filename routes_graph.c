#include <stdio.h>
#include <string.h>
#include "routes_graph.h"

/* FUNCIÓN AUXILIAR — buscar índice de un punto por nombre
   Entrada:  g    — puntero al grafo
           name — nombre del punto a buscar
   Salida: índice en el arreglo, o -1 si no existe */
static int find_index(const Graph *g, const char *name) {
    for (int i = 0; i < g->count; i++) {
        if (strcmp(g->points[i], name) == 0) return i;
    }
    return -1;
}

/* INICIALIZAR GRAFO
   entrada:  g — puntero al grafo a inicializar
   salida:  pone todos los pesos en 0 y el contador en 0 */
void graph_init(Graph *g) {
    g->count = 0;
    for (int i = 0; i < MAX_POINTS; i++) {
        g->points[i][0] = '\0';
        for (int j = 0; j < MAX_POINTS; j++) {
            g->matrix[i][j] = 0;
        }
    }
}

/* REGISTRAR PUNTO DE ENTREGA
   Entrada:  g    — puntero al grafo
           name — nombre del nuevo punto (ej: "San Jose Centro")
   Salida: índice asignado, o -1 si falla
   Valida: no duplicados, no superar MAX_POINTS*/
int graph_add_point(Graph *g, const char *name) {
    if (g->count >= MAX_POINTS) {
        printf("Error: se alcanzo el maximo de puntos (%d).\n", MAX_POINTS);
        return -1;
    }
    if (find_index(g, name) != -1) {
        printf("Error: el punto '%s' ya existe.\n", name);
        return -1;
    }
    strncpy(g->points[g->count], name, MAX_STR - 1);
    g->points[g->count][MAX_STR - 1] = '\0';
    return g->count++;
}

/* REGISTRAR RUTA ENTRE DOS PUNTOS
   entrada:  g      — puntero al grafo
           from   — nombre del punto origen
           to     — nombre del punto destino
           weight — distancia o tiempo de la ruta (debe ser > 0)
   salida: 0 si éxito, -1 si falla
   Valida: ambos puntos deben existir, peso positivo */
int graph_add_route(Graph *g, const char *from, const char *to, int weight) {
    int i = find_index(g, from);
    int j = find_index(g, to);

    if (i == -1) {
        printf("Error: el punto origen '%s' no existe.\n", from);
        return -1;
    }
    if (j == -1) {
        printf("Error: el punto destino '%s' no existe.\n", to);
        return -1;
    }
    if (weight <= 0) {
        printf("Error: el peso de la ruta debe ser mayor que 0.\n");
        return -1;
    }

    g->matrix[i][j] = weight;
    return 0;
}

/* MOSTRAR PUNTOS DE ENTREGA */
void graph_show_points(const Graph *g) {
    if (g->count == 0) {
        printf("No hay puntos de entrega registrados.\n");
        return;
    }
    printf("Puntos de entrega registrados (%d):\n", g->count);
    for (int i = 0; i < g->count; i++) {
        printf("  [%d] %s\n", i, g->points[i]);
    }
}

/* MOSTRAR MATRIZ DE ADYACENCIA
   Imprime la tabla origen × destino con los pesos.
   0 = no hay ruta directa entre esos dos puntos. */
void graph_show_matrix(const Graph *g) {
    if (g->count == 0) {
        printf("El grafo no tiene puntos aun.\n");
        return;
    }

    /* Encabezado de columnas */
    printf("\n%20s", " ");
    for (int j = 0; j < g->count; j++) {
        printf("%12s", g->points[j]);
    }
    printf("\n");

    /* Separador */
    printf("%20s", " ");
    for (int j = 0; j < g->count; j++) printf("------------");
    printf("\n");

    /* Filas */
    for (int i = 0; i < g->count; i++) {
        printf("%20s |", g->points[i]);
        for (int j = 0; j < g->count; j++) {
            if (g->matrix[i][j] == 0)
                printf("%12s", "-");
            else
                printf("%12d", g->matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
