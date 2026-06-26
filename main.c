#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── Módulos del proyecto ───────────────────────────────────────── */
#include "client_bst.h"
#include "package_avl.h"
#include "routes_graph.h"
#include "files.h"
#include "package_list.h"
#include "delivery_queue.h"
#include "returns_stack.h"

/* ── Estado global ──────────────────────────────────────────────── */
BSTNode    *client_tree  = NULL;
AVLNode    *package_tree = NULL;
Graph       route_graph;
NodoPaquete *lista_paquetes = NULL;
Cola        cola_entregas;
Pila        pila_devoluciones;

/* REGISTRAR PAQUETE  */
void register_package() {
    Paquete p;

    printf("\n--- Registrar nuevo paquete ---\n");
    printf("Codigo del paquete: ");
    scanf(" %[^\n]", p.codigo);

    printf("ID del cliente: ");
    scanf("%d", &p.idCliente);

    /* VALIDACIÓN: el cliente debe existir en el BST */
    BSTNode *found = bst_search(client_tree, p.idCliente);
    if (found == NULL) {
        printf("Error: no existe un cliente con ID %d. Paquete rechazado.\n", p.idCliente);
        return;
    }

    printf("Destinatario: ");
    scanf(" %[^\n]", p.destinatario);

    printf("Destino: ");
    scanf(" %[^\n]", p.destino);

    printf("Peso (kg): ");
    scanf("%f", &p.peso);

    printf("Prioridad (1=alta, 2=media, 3=baja): ");
    scanf("%d", &p.prioridad);

    strcpy(p.estado, "pendiente");

    /* Agregar a lista enlazada y encolar */
    lista_paquetes = registrarPaquete(lista_paquetes, p);
    encolarPaquete(&cola_entregas, p);

    printf("Paquete registrado y encolado para el cliente %s.\n", found->data.name);
}

/*  MENÚ PRINCIPAL */
    /* Inicializar estructuras */
    graph_init(&route_graph);
    inicializarCola(&cola_entregas);
    inicializarPila(&pila_devoluciones);

    /* Cargar datos desde archivos */
    client_tree = load_clients(client_tree);

    int option = -1;

    while (option != 0) {
        printf("\n========== RoutePack ==========\n");
        printf("1. Gestion de clientes (BST)\n");
        printf("2. Gestion de paquetes (Lista + AVL)\n");
        printf("3. Cola de entregas\n");
        printf("4. Pila de devoluciones\n");
        printf("5. Rutas y navegacion\n");
        printf("6. Reportes\n");
        printf("0. Salir\n");
        printf("Opcion: ");
        scanf("%d", &option);

        switch (option) {

            /* ── Clientes (BST) ─ */
            case 1: {
                int sub = -1;
                printf("\n1. Registrar cliente\n2. Buscar cliente\n3. Listar clientes (inorden)\nOpcion: ");
                scanf("%d", &sub);
                if (sub == 1) {
                    Client c;
                    printf("ID: ");        scanf("%d",  &c.id);
                    printf("Nombre: ");    scanf(" %[^\n]", c.name);
                    printf("Telefono: ");  scanf(" %[^\n]", c.phone);
                    printf("Correo: ");    scanf(" %[^\n]", c.email);
                    printf("Direccion: "); scanf(" %[^\n]", c.address);
                    client_tree = bst_insert(client_tree, c);
                } else if (sub == 2) {
                    int id;
                    printf("ID a buscar: "); scanf("%d", &id);
                    BSTNode *r = bst_search(client_tree, id);
                    if (r) printf("Encontrado: %s | %s | %s\n", r->data.name, r->data.phone, r->data.email);
                    else   printf("Cliente no encontrado.\n");
                } else if (sub == 3) {
                    bst_inorder(client_tree);
                }
                break;
            }

            /* ── Paquetes (Lista enlazada + AVL) ──────────────── */
            case 2: {
                int sub = -1;
                printf("\n1. Registrar paquete\n2. Buscar paquete\n3. Listar paquetes\n4. Modificar estado\n5. Eliminar paquete\nOpcion: ");
                scanf("%d", &sub);
                if (sub == 1) {
                    register_package();
                } else if (sub == 2) {
                    char codigo[20];
                    printf("Codigo a buscar: "); scanf(" %[^\n]", codigo);
                    NodoPaquete *r = buscarPaquete(lista_paquetes, codigo);
                    if (r) printf("Encontrado: %s | %s | Estado: %s\n", r->datos.codigo, r->datos.destino, r->datos.estado);
                    else   printf("Paquete no encontrado.\n");
                } else if (sub == 3) {
                    mostrarPaquetes(lista_paquetes);
                } else if (sub == 4) {
                    char codigo[20], estado[30];
                    printf("Codigo: ");      scanf(" %[^\n]", codigo);
                    printf("Nuevo estado: "); scanf(" %[^\n]", estado);
                    modificarEstadoPaquete(lista_paquetes, codigo, estado);
                } else if (sub == 5) {
                    char codigo[20];
                    printf("Codigo a eliminar: "); scanf(" %[^\n]", codigo);
                    lista_paquetes = eliminarPaquete(lista_paquetes, codigo);
                }
                break;
            }

            /* ── Cola de entregas (FIFO) ──────────────────────── */
            case 3: {
                int sub = -1;
                printf("\n1. Ver cola de entregas\n2. Consultar siguiente\n3. Procesar entrega\nOpcion: ");
                scanf("%d", &sub);
                if (sub == 1) {
                    mostrarCola(cola_entregas);
                } else if (sub == 2) {
                    consultarSiguienteCola(cola_entregas);
                } else if (sub == 3) {
                    /* Desencolar y marcar como entregado o devuelto */
                    if (cola_entregas.frente == NULL) {
                        printf("No hay paquetes en la cola.\n");
                    } else {
                        Paquete procesado = desencolarPaquete(&cola_entregas);
                        int resultado;
                        printf("Entrega exitosa? (1=Si / 0=No): ");
                        scanf("%d", &resultado);
                        if (resultado) {
                            modificarEstadoPaquete(lista_paquetes, procesado.codigo, "entregado");
                            printf("Paquete %s marcado como entregado.\n", procesado.codigo);
                        } else {
                            strcpy(procesado.estado, "devuelto");
                            modificarEstadoPaquete(lista_paquetes, procesado.codigo, "devuelto");
                            apilarPaquete(&pila_devoluciones, procesado);
                            printf("Paquete %s enviado a pila de devoluciones.\n", procesado.codigo);
                        }
                    }
                }
                break;
            }

            /* ── Pila de devoluciones (LIFO) ──────────────────── */
            case 4: {
                int sub = -1;
                printf("\n1. Ver pila de devoluciones\n2. Consultar ultimo devuelto\n3. Reencolar devolucion\nOpcion: ");
                scanf("%d", &sub);
                if (sub == 1) {
                    mostrarPila(pila_devoluciones);
                } else if (sub == 2) {
                    consultarUltimoDevuelto(pila_devoluciones);
                } else if (sub == 3) {
                    /* Desapilar y reencolar */
                    if (pila_devoluciones.tope == NULL) {
                        printf("No hay paquetes devueltos.\n");
                    } else {
                        Paquete reencolar = desapilarPaquete(&pila_devoluciones);
                        strcpy(reencolar.estado, "pendiente");
                        modificarEstadoPaquete(lista_paquetes, reencolar.codigo, "pendiente");
                        encolarPaquete(&cola_entregas, reencolar);
                        printf("Paquete %s reencolado.\n", reencolar.codigo);
                    }
                }
                break;
            }

            /* ── Rutas y navegación ───────────────────────────── */
            case 5: {
                int sub = -1;
                printf("\n1. Agregar punto\n2. Agregar ruta\n3. Ver puntos\n4. Ver matriz\nOpcion: ");
                scanf("%d", &sub);
                if (sub == 1) {
                    char name[MAX_STR];
                    printf("Nombre del punto: "); scanf(" %[^\n]", name);
                    graph_add_point(&route_graph, name);
                } else if (sub == 2) {
                    char from[MAX_STR], to[MAX_STR]; int w;
                    printf("Origen: ");    scanf(" %[^\n]", from);
                    printf("Destino: ");   scanf(" %[^\n]", to);
                    printf("Distancia: "); scanf("%d", &w);
                    graph_add_route(&route_graph, from, to, w);
                } else if (sub == 3) {
                    graph_show_points(&route_graph);
                } else if (sub == 4) {
                    graph_show_matrix(&route_graph);
                }
                break;
            }

            /* ── Reportes ─────────────────────────────────────── */
            case 6: {
                int sub = -1;
                printf("\n1. Paquetes por estado\n2. Clientes (BST inorden)\n3. Ruta mas corta (Dijkstra)\nOpcion: ");
                scanf("%d", &sub);
                if (sub == 1) {
                    printf("Pendientes:  %d\n", contarPaquetesPorEstado(lista_paquetes, "pendiente"));
                    printf("Entregados:  %d\n", contarPaquetesPorEstado(lista_paquetes, "entregado"));
                    printf("Devueltos:   %d\n", contarPaquetesPorEstado(lista_paquetes, "devuelto"));
                } else if (sub == 2) {
                    bst_inorder(client_tree);
                } else if (sub == 3) {
                    char origen[MAX_STR], destino[MAX_STR];
                    graph_show_points(&route_graph);
                    printf("Punto origen: ");  scanf(" %[^\n]", origen);
                    printf("Punto destino: "); scanf(" %[^\n]", destino);
                    graph_shortest_path(&route_graph, origen, destino);
                }
                break;
            }

            /* ── Salir ────────────────────────────────────────── */
            case 0:
                save_clients(client_tree);
                printf("Hasta luego.\n");
                break;

            default:
                printf("Opcion invalida.\n");
        }
    }

    /* Liberar memoria */
    bst_free(client_tree);
    avl_free(package_tree);
    return 0;
}