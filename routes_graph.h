#ifndef ROUTES_GRAPH_H
#define ROUTES_GRAPH_H

#define MAX_POINTS 20   
#define MAX_STR 100

typedef struct {
    char points[MAX_POINTS][MAX_STR];   
    int matrix[MAX_POINTS][MAX_POINTS];  
    int count;                           
} Graph;

void graph_init(Graph *g);

int graph_add_point(Graph *g, const char *name);

int graph_add_route(Graph *g, const char *from, const char *to, int weight);

void graph_show_points(const Graph *g);

void graph_show_matrix(const Graph *g);

void graph_shortest_path(const Graph *g, const char *from, const char *to);

#endif 