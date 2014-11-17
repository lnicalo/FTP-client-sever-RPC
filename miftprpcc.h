// constantes para el programa
#define COM_MAX 256 	/* numero maximo de caracteres por comando */
#define MAX_ARG 15 	/* numero maximo de argumentos en un comando */
#define CAD_MAX 256 	/* numero bytes reservados para almacanar el login y el password */
#define TAM_BUFFER 1024  /*Tamaño del buffer*/

#include "ftprpc.h"
#include "librerias.h"
void Autenticacion();
int ControlReplicas (char * mensaje);
void Interfaz(CLIENT *clnt);
// Comandos
int ejecutar_put(char **args, CLIENT *clnt);
int ejecutar_get(char **args, CLIENT *clnt);
void ejecutar_cd(char **args, CLIENT *clnt);
void ejecutar_rm(char **args, CLIENT *clnt);
int ejecutar_list(char **args, CLIENT *clnt);
