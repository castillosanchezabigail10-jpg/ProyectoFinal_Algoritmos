#ifndef DELIVERY_QUEUE_H
#define DELIVERY_QUEUE_H

#include "package_list.h" 

typedef struct NodoCola {
    Paquete datos;
    struct NodoCola* siguiente;
} NodoCola;

typedef struct {
    NodoCola* frente;
    NodoCola* final;
} Cola;

void inicializarCola(Cola* c);

void encolarPaquete(Cola* c, Paquete nuevoPaquete);

void mostrarCola(Cola c);

void consultarSiguienteCola(Cola c);

Paquete desencolarPaquete(Cola* c);

#endif