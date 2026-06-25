#ifndef RETURNS_STACK_H
#define RETURNS_STACK_H

#include "package_list.h" 



typedef struct NodoPila {
    Paquete datos;
    struct NodoPila* siguiente;
} NodoPila;

typedef struct {
    NodoPila* tope;
} Pila;

void inicializarPila(Pila* p);

void apilarPaquete(Pila* p, Paquete paqueteDevuelto);

void mostrarPila(Pila p);

void consultarUltimoDevuelto(Pila p);

Paquete desapilarPaquete(Pila* p);

#endif 