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

#include <limits.h>

#define INF INT_MAX

/* FUNCIÓN AUXILIAR INTERNA — Encuentra el vértice con la distancia mínima 
   que aún no ha sido procesado (visitado). */
static int dijkstra_min_distance(const int dist[], const int visited[], int count) {
    int min = INF, min_index = -1;

    for (int v = 0; v < count; v++) {
        if (visited[v] == 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

/* FUNCIÓN AUXILIAR INTERNA*/
static void dijkstra_print_path(const int parent[], int j, const Graph *g) {
    if (parent[j] == -1) {
        printf("%s", g->points[j]);
        return;
    }

    dijkstra_print_path(parent, parent[j], g);
    printf(" -> %s", g->points[j]);
}

/* CALCULAR RUTA MÁS CORTA (DIJKSTRA)*/
void graph_shortest_path(const Graph *g, const char *from, const char *to) {
    int count = g->count;
    int src = -1, dest = -1;

    for (int i = 0; i < count; i++) {
        if (strcmp(g->points[i], from) == 0) src = i;
        if (strcmp(g->points[i], to) == 0) dest = i;
    }

    if (src == -1) {
        printf("Error: El punto de origen '%s' no existe.\n", from);
        return;
    }
    if (dest == -1) {
        printf("Error: El punto de destino '%s' no existe.\n", to);
        return;
    }

    int dist[MAX_POINTS];     
    int visited[MAX_POINTS]; 
    int parent[MAX_POINTS];   

    for (int i = 0; i < count; i++) {
        dist[i] = INF;
        visited[i] = 0;
        parent[i] = -1;
    }

    dist[src] = 0;

    for (int i = 0; i < count - 1; i++) {
        int u = dijkstra_min_distance(dist, visited, count);

        if (u == -1 || dist[u] == INF) break;

        visited[u] = 1;

        for (int v = 0; v < count; v++) {

            if (!visited[v] && g->matrix[u][v] > 0 && dist[u] != INF 
                && dist[u] + g->matrix[u][v] < dist[v]) {
                
                parent[v] = u;
                dist[v] = dist[u] + g->matrix[u][v];
            }
        }
    }

    if (dist[dest] == INF) {
        printf("No existe una ruta disponible entre '%s' y '%s'.\n", from, to);
    } else {
        printf("\nRuta mas corta: ");
        dijkstra_print_path(parent, dest, g);
        printf("\n");
        printf("Distancia total: %d km\n\n", dist[dest]);
    }
}