#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- ESTRUCTURAS COMPARTIDAS ---
typedef struct {
    char codigo[20];
    int idCliente;
    char destinatario[100];
    char destino[200];
    float peso;
    int prioridad;
    char estado[30]; 
} Paquete;

typedef struct NodoCola {
    Paquete datos;
    struct NodoCola* siguiente;
} NodoCola;

typedef struct {
    NodoCola* frente;
    NodoCola* final;
} Cola;


// Inicializa la cola apuntando sus extremos a NULL
void inicializarCola(Cola* c) {
    c->frente = NULL;
    c->final = NULL;
}

// Encolar un paquete registrado (Comportamiento FIFO)
void encolarPaquete(Cola* c, Paquete nuevoPaquete) {
    NodoCola* nuevoNodo = (NodoCola*)malloc(sizeof(NodoCola));
    if (nuevoNodo == NULL) {
        printf("Error: No hay memoria para encolar el paquete.\n");
        return;
    }

    // Restricción: Forzar el estado a "Pendiente de entrega"
    strcpy(nuevoPaquete.estado, "Pendiente de entrega");
    nuevoNodo->datos = nuevoPaquete;
    nuevoNodo->siguiente = NULL;

    if (c->final == NULL) {
        c->frente = nuevoNodo;
        c->final = nuevoNodo;
    } else {
        c->final->siguiente = nuevoNodo;
        c->final = nuevoNodo;
    }
    printf("Paquete %s encolado con exito.\n", nuevoPaquete.codigo);
}

// Mostrar todos los paquetes en la cola
void mostrarCola(Cola c) {
    if (c.frente == NULL) {
        printf("La cola de entregas pendientes esta vacia.\n");
        return;
    }

    NodoCola* actual = c.frente;
    printf("\n--- COLA DE ENTREGAS PENDIENTES ---\n");
    while (actual != NULL) {
        printf("Codigo: %s | Destinatario: %s | Estado: %s\n",
               actual->datos.codigo, actual->datos.destinatario, actual->datos.estado);
        actual = actual->siguiente;
    }
}

// Consultar el siguiente paquete en la cola sin sacarlo
void consultarSiguienteCola(Cola c) {
    if (c.frente == NULL) {
        printf("No hay paquetes pendientes en la cola.\n");
        return;
    }
    printf("Siguiente en cola: %s (Destinatario: %s)\n", 
           c.frente->datos.codigo, c.frente->datos.destinatario);
}

// Desencolar un paquete para su procesamiento
// Retorna la estructura con los datos del paquete extraído
Paquete desencolarPaquete(Cola* c) {
    Paquete pkgVacio = {"", 0, "", "", 0.0, 0, ""}; // Por si está vacía
    
    if (c->frente == NULL) {
        printf("Error: La cola esta vacia, no se puede desencolar.\n");
        return pkgVacio;
    }

    NodoCola* temporal = c->frente;
    Paquete datosPaquete = temporal->datos;

    c->frente = c->frente->siguiente;
    if (c->frente == NULL) {
        c->final = NULL;
    }

    free(temporal);
    return datosPaquete;
} 