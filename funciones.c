/************************************************************************ 
Fichero: funciones.c 
Programa: miftp
Otros ficheros del programa:  miftp.c, funciones.c, comandos.c, 
			      colaboradoras.c, miftp.h
Versi�n: 1.5 Fecha: 29-03-2009 
Autor/es: Luis Fernando Nicolas Alonso, lnicalo@ribera.tel.uva.es
	  Javier Cortejoso de Andr�s, jcorand@ribera.tel.uva.es
	  Grupo 2 de Fundamentos de Ordenadores, Ing. Telecomunicaciones
Descripcion: Cliente ftp sencillo basado en el uso de sockets.
Objetivo: Dise�o de un programa-cliente capaz de conectarse con un ser-
	  vidor que siga el estadar ftp descrito en la rfc 793, ser au-
	  tentificado de forma correcta, y ser capaz de enviar y reci-
	  bir archivos, as� como otras funciones menos relevantes.
*************************************************************************/
#include "miftprpcc.h"
#include "ftprpc.h"

/*--------------------------------------------------------------------------------------------------
Funcion: Autenticacion()
Sintaxis: void Autenticacion(struct sUser *autenticacion_rmt_1_arg)
Prototipo en: 
Argumentos: struct sUser *autenticacion_rmt_1_arg: Puntero a estructura sUser con datos de usuario
Descripcion: Recoge el login y el password del usuario local	
Llama a: 
Fecha ultima modificacion: 10-04-09 
Ultima modificacion: Desarrollo de la funcion
---------------------------------------------------------------------------------------------------*/

void Autenticacion(CLIENT *clnt)	{
	
	static struct sUser autenticacion_rmt_1_arg;
	char *user;
	char * *result_1;
	int veces = 0;
	int exito = 0;
	char *passwd;
	if ((user = (char *) getenv("LOGNAME")) == NULL) {
		fprintf(stderr,"Error al intentar averiguar el nombre del usuario\n");
		perror("mirpc");
	}
	
	do {
		printf("Nombre(%s):",user);
	
		/* Recogemos cadena */
		if(fgets(autenticacion_rmt_1_arg.login, CAD_MAX, stdin) == NULL) {
			fprintf(stderr,"Error en la lectura del login\n");
			exit(1);
		}
		autenticacion_rmt_1_arg.login[strlen(autenticacion_rmt_1_arg.login)-1] = 0;
		if (strlen(autenticacion_rmt_1_arg.login) == 0) {
			strcpy(autenticacion_rmt_1_arg.login,user);
		}
		passwd  = (char *) getpass("Password:");
		if (passwd == NULL ) {
			fprintf(stderr,"Error al leer el password\n");
			perror("miftprpc");
			exit(1);
		}
		strcpy(autenticacion_rmt_1_arg.passwd,passwd);
		result_1 = autenticacion_rmt_1(&autenticacion_rmt_1_arg, clnt);
		if (result_1 == (char **) NULL) {
			clnt_perror(clnt, "call failed");
		}
		printf("%s",*result_1);
		
		if (ControlReplicas (*result_1) != 2)	{
			++veces;
		}
		else {
			exito = 1;
		}
	} while ((exito != 1) && (veces < 3));
	
	if (exito == 0) {
		printf ("Se agoto el numero maximo numero de intentos en la identificacion\n");
		exit (0);
	}
		
}

/*--------------------------------------------------------------------------------------------------
Funcion: ControlReplicas()
Sintaxis: int Autenticacion(char * mensaje)
Prototipo en: rpcftp.h
Argumentos: char * mensaje: puntero a cadena de caracteres que contiene los n�umeros de la replica
Descripcion: Hace un control de la r�eplica enviada por el servidor. Es un control simple, solo 
	     comprobando en primer numero de la replica.
Llama a:
Fecha ultima modificacion: 11-04-09
Ultima modificacion: Desarrollo de la funcion
---------------------------------------------------------------------------------------------------*/

int ControlReplicas(char *mensaje) {
        char digitochar[1];
        int digito;
        strncpy (digitochar, mensaje, 1);
        digito = atoi (digitochar); 
        switch (digito) {
                case 1:
                        return (1);
                case 2:
                        return (2);
                case 3:
                        return (3);
                case 4:
                        return (4);
                case 5:
                        return (5);
                }
}

/*--------------------------------------------------------------------------------------------------
Funcion: Interfaz()                                                                                 
Sintaxis: void Interfaz(CLIENT clnt)                                                                
Prototipo en: rpcftp.h                                                                              
Argumentos: CLIENT clnt: Cliente                                                                    
Descripcion: Funcion interfaz.                                                                      
Llama a: ejecutar_get, ejecutar_put.                                                                
Fecha ultima modificacion: 11-04-09                                                                 
Ultima modificacion: Desarrollo de la funcion                                                       
---------------------------------------------------------------------------------------------------*/

void Interfaz(CLIENT *clnt)
{                         
        char comando[COM_MAX];  // Cadena donde se almacena el comando introducido por el usuario
        char *saveptr;          //                                                               
        char *token[MAX_ARG];   // Vector de punteros a los distintos argumentos de los comandos 
        int j;                  // Variable de control para el bucle que se hace en la separacion de los argumentos del comando                                                                                                                           
        int bytes;              // Bytes transmitidos en los comandos que requieren conexion de datos                        

        while (1)               {
             inicio:    printf ("miftprpc> ");
                        // Recogemos la cadena introducida, y hacemos una limpieza de buffer de stdin por si tuviera 
                        // residuos                                                                                  
                        fflush (stdin);                                                                              
                        if(fgets(comando, COM_MAX, stdin) == NULL) {                                                 
                                fprintf(stderr,"Error en la lectura del comando\n");                                 
                                exit(1);                                                                             
                        }                                                                                            
                        // quitamos el retorno de linea recogido por fgets                                           
                        comando[strlen(comando)-1] = 0;                                                              
                        // Hay que asegurarse de que no se ha dado el intro sin haber introducido un comando         
                        // en ese caso *comando ser�a nulo                                                           
                        if (*comando != 0)      {                                                                    
                                // solo separamos los  cuando no es comando local, es decir en el primer caacter no tenemos un !                                                                                                                          
                                if (*comando != '!') {                                                                       
                                        bzero(token,MAX_ARG*sizeof(char *));                                                 
                                        // separamos la cadena. Utilizando como separador el espacio en blanco               
                                        j=0;                                                                                 
                                        if ((token[j] = (char *)strtok_r(comando, " ", &saveptr)) != NULL) {                 
                                                j++;                                                                         
                                                while (((token[j] = (char *)strtok_r(NULL, " ", &saveptr)) != NULL) && 	(j < MAX_ARG))  {
                                                        j++;
                                                        if (j == MAX_ARG) {
                                                                fprintf(stdout,"Demasiados argumentos\n");
                                                               	// No es conveniente usar goto pero en este caso creo que es la mejor opcion
								goto inicio;
                                                        }
                                                }
                                        }
                                }

                                //Sabemos que tiene que el usuario tiene que introducir lo primero es el comando
                                // No comprobamos los argumentos (token) aqui ya lo hacemos en las funciones que ejecutan los
                                // comando porque son las que saben si los argumentos son correctos
                                if (!strcmp(token[0], "get")) {
                                        bytes = ejecutar_get(token, clnt);
                                        // mostrar bytes recibidos si se ha recibido al menos 1
                                        if (bytes > 0 ) {
                                                fprintf(stdout,"bytes recibidos %d\n",bytes);
                                        }
                                }
                                else if (!strcmp(token[0],"put")) {
                                        bytes = ejecutar_put(token, clnt);
                                        // mostrar bytes enviados si se ha enviado al menos 1
                                        if (bytes > 0 ) {
                                                fprintf(stdout,"bytes enviados: %d\n",bytes);
                                        }
                                }
				else if (!strcmp(token[0],"cd")) {
					ejecutar_cd(token, clnt);
				}
				else if (!strcmp(token[0],"borrar")){
					ejecutar_rm(token, clnt);
				}
				else if (!strcmp(token[0],"list")) {
					bytes = ejecutar_list(token, clnt);
                                        if (bytes > 0 ) {
                                                fprintf(stdout,"bytes recibidos %d\n",bytes);
                                        }
				}
				else if (!strcmp(token[0],"quit")) {
					break;
				}
                                else    {
                                        printf("Comando invalido\n");
                                }

                        }
        }
}

