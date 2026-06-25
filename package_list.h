#ifndef PACKAGE_LIST_H
#define PACKAGE_LIST_H

typedef struct {
    char codigo[20];          
    int idCliente;            
    char destinatario[100];
    char destino[200];
    float peso;
    int prioridad;            
    char estado[30];          
} Paquete;


typedef struct NodoPaquete {
    Paquete datos;
    struct NodoPaquete* siguiente;
} NodoPaquete;

NodoPaquete* buscarPaquete(NodoPaquete* cabeza, const char* codigoBuscar);

NodoPaquete* registrarPaquete(NodoPaquete* cabeza, Paquete nuevoPaquete);


void mostrarPaquetes(NodoPaquete* cabeza);


void modificarEstadoPaquete(NodoPaquete* cabeza, const char* codigoBuscar, const char* nuevoEstado);

NodoPaquete* eliminarPaquete(NodoPaquete* cabeza, const char* codigoEliminar);


int contarPaquetesPorEstado(NodoPaquete* cabeza, const char* estadoBuscar);

#endif