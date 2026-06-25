#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char codigo[20];
    int idCliente;
    char destinatario[100];
    char destino[200];
    float peso;
    int prioridad;
    char estado[30]; 
} Paquete;

typedef struct NodoPila {
    Paquete datos;
    struct NodoPila* siguiente;
} NodoPila;

typedef struct {
    NodoPila* tope;
} Pila;

// --- FUNCIONALIDADES DE LA PILA ---
// Inicializa la pila con el tope apuntando a NULL
void inicializarPila(Pila* p) {
    p->tope = NULL;
}

//Apilar un paquete devuelto (Comportamiento LIFO)
void apilarPaquete(Pila* p, Paquete paqueteDevuelto) {
    NodoPila* nuevoNodo = (NodoPila*)malloc(sizeof(NodoPila));
    if (nuevoNodo == NULL) {
        printf("Error: No hay memoria para apilar el paquete devuelto.\n");
        return;
    }

    // Forzar el estado a "Devuelto" al ingresar a la pila
    strcpy(paqueteDevuelto.estado, "Devuelto");
    nuevoNodo->datos = paqueteDevuelto;
    
    // El nuevo nodo apunta al antiguo tope y se convierte en el nuevo tope
    nuevoNodo->siguiente = p->tope;
    p->tope = nuevoNodo;
    
    printf("Paquete %s almacenado en la Pila de Devoluciones.\n", paqueteDevuelto.codigo);
}

//Mostrar los paquetes devueltos en la pila
void mostrarPila(Pila p) {
    if (p.tope == NULL) {
        printf("La pila de devoluciones esta vacia.\n");
        return;
    }

    NodoPila* actual = p.tope;
    printf("\n--- PILA DE PAQUETES DEVUELTOS (Ultimo en entrar, primero en salir) ---\n");
    while (actual != NULL) {
        printf("Codigo: %s | Destinatario: %s | Motivo: %s\n",
               actual->datos.codigo, actual->datos.destinatario, actual->datos.estado);
        actual = actual->siguiente;
    }
}

//Consultar el ultimo paquete devuelto (Ver el tope sin sacarlo)
void consultarUltimoDevuelto(Pila p) {
    if (p.tope == NULL) {
        printf("No hay paquetes devueltos en la pila para consultar.\n");
        return;
    }
    printf("Ultimo devuelto en el tope: %s (Destinatario: %s)\n", 
           p.tope->datos.codigo, p.tope->datos.destinatario);
}

// Desapilar un paquete para reprocesarlo e interconectarlo
// Retorna el paquete para que en el main puedas enviarlo directo a la cola
Paquete desapilarPaquete(Pila* p) {
    Paquete pkgVacio = {"", 0, "", "", 0.0, 0, ""};
    
    if (p->tope == NULL) {
        printf("Error: No hay paquetes en la pila para desapilar.\n");
        return pkgVacio;
    }

    NodoPila* temporal = p->tope;
    Paquete paqueteAMover = temporal->datos;

    // El tope ahora pasa a ser el siguiente de la lista
    p->tope = p->tope->siguiente;
    free(temporal);

    printf("Paquete %s extraido de la pila de devoluciones.\n", paqueteAMover.codigo);
    return paqueteAMover;
}