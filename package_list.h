#ifndef PACKAGE_LIST_H
#define PACKAGE_LIST_H

// --- ESTRUCTURAS ---

// Definición de la estructura para los datos del Paquete
typedef struct {
    char codigo[20];          
    int idCliente;            
    char destinatario[100];
    char destino[200];
    float peso;
    int prioridad;            
    char estado[30];          
} Paquete;

// Definición del Nodo de la Lista Enlazada Simple
typedef struct NodoPaquete {
    Paquete datos;
    struct NodoPaquete* siguiente;
} NodoPaquete;

// --- PROTOTIPOS DE LAS FUNCIONES ---

// Función auxiliar para buscar si un código de paquete ya existe
NodoPaquete* buscarPaquete(NodoPaquete* cabeza, const char* codigoBuscar);

// Registrar paquetes sin permitir códigos duplicados (Insertar al inicio de la lista)
NodoPaquete* registrarPaquete(NodoPaquete* cabeza, Paquete nuevoPaquete);

// Mostrar la lista completa de paquetes
void mostrarPaquetes(NodoPaquete* cabeza);

// Modificar el estado de un paquete
void modificarEstadoPaquete(NodoPaquete* cabeza, const char* codigoBuscar, const char* nuevoEstado);

// Eliminar paquetes de la lista
NodoPaquete* eliminarPaquete(NodoPaquete* cabeza, const char* codigoEliminar);

// Contar paquetes por estado para reportes
int contarPaquetesPorEstado(NodoPaquete* cabeza, const char* estadoBuscar);

#endif // PACKAGE_LIST_H