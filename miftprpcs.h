// constantes para el programa
#define TAM_BUFFER 1024  /*Tamaño del buffer*/
#define TAM_REPLICA 1024  /*Tamaño del buffer*/
#define MAX_ARG 15      /* numero maximo de argumentos en un comando */
#include "ftprpc.h"
#include "librerias.h"

void Autenticacion();
int ControlReplicas (char * mensaje);

// Comandos
int ejecutar_put(char **args, CLIENT *clnt);
