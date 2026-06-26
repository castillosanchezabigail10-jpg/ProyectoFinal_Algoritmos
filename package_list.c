#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "package_list.h"

// Función auxiliar para buscar si un código de paquete ya existe
NodoPaquete* buscarPaquete(NodoPaquete* cabeza, const char* codigoBuscar) {
    NodoPaquete* actual = cabeza;
    
    while (actual != NULL) {
        if (strcmp(actual->datos.codigo, codigoBuscar) == 0) {
            return actual; 
        }
        actual = actual->siguiente;
    }
    return NULL; 
}

// Registrar paquetes sin permitir códigos duplicados (Insertar al inicio de la lista)
NodoPaquete* registrarPaquete(NodoPaquete* cabeza, Paquete nuevoPaquete) {
    if (buscarPaquete(cabeza, nuevoPaquete.codigo) != NULL) {
        printf("Error: El paquete con codigo %s ya esta registrado.\n", nuevoPaquete.codigo);
        return cabeza; 
    }


    NodoPaquete* nuevoNodo = (NodoPaquete*)malloc(sizeof(NodoPaquete));
    if (nuevoNodo == NULL) {
        printf("Error: No se pudo asignar memoria para el paquete.\n");
        return cabeza;
    }

    nuevoNodo->datos = nuevoPaquete;
    nuevoNodo->siguiente = cabeza;

    printf("Paquete %s registrado con exito.\n", nuevoPaquete.codigo);
    
    return nuevoNodo;
}

// Mostrar la lista completa de paquetes
void mostrarPaquetes(NodoPaquete* cabeza) {
    if (cabeza == NULL) {
        printf("La lista de paquetes esta vacia.\n");
        return;
    }

    NodoPaquete* actual = cabeza;
    printf("\n=== LISTA DE PAQUETES REGISTRADOS ===\n");
    while (actual != NULL) {
        printf("Codigo: %s | Cliente: %d | Destinatario: %s | Destino: %s | Peso: %.2fkg | Prioridad: %d | Estado: %s\n",
               actual->datos.codigo, actual->datos.idCliente, actual->datos.destinatario, 
               actual->datos.destino, actual->datos.peso, actual->datos.prioridad, actual->datos.estado);
        actual = actual->siguiente;
    }
    printf("======================================\n");
}

// Modificar el estado de un paquete
void modificarEstadoPaquete(NodoPaquete* cabeza, const char* codigoBuscar, const char* nuevoEstado) {
    NodoPaquete* encontrado = buscarPaquete(cabeza, codigoBuscar);
    
    if (encontrado != NULL) {
        strcpy(encontrado->datos.estado, nuevoEstado);
        printf("Estado del paquete %s actualizado a: %s\n", codigoBuscar, nuevoEstado);
    } else {
        printf("Error: No se encontro el paquete con codigo %s.\n", codigoBuscar);
    }
}

//Eliminar paquetes de la lista
NodoPaquete* eliminarPaquete(NodoPaquete* cabeza, const char* codigoEliminar) {
    NodoPaquete* actual = cabeza;
    NodoPaquete* anterior = NULL;

    while (actual != NULL && strcmp(actual->datos.codigo, codigoEliminar) != 0) {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual == NULL) {
        printf("Error: El paquete con codigo %s no existe.\n", codigoEliminar);
        return cabeza;
    }

    if (anterior == NULL) {
        cabeza = actual->siguiente;
    } else {
        anterior->siguiente = actual->siguiente;
    }

    free(actual);
    printf("Paquete %s eliminado correctamente del sistema.\n", codigoEliminar);

    return cabeza;
}

// Contar paquetes por estado para reportes
int contarPaquetesPorEstado(NodoPaquete* cabeza, const char* estadoBuscar) {
    int contador = 0;
    NodoPaquete* actual = cabeza;

    while (actual != NULL) {
        if (strcmp(actual->datos.estado, estadoBuscar) == 0) {
            contador++;
        }
        actual = actual->siguiente;
    }

    return contador;
}