#include "miftprpcc.h"

/*--------------------------------------------------------------------------------------------------
Funcion: ejecutar_put()
Sintaxis: int ejecutar_put(char **args, CLIENT *clnt;)
Prototipo en: rpcftp.h
Argumentos: char **args:  vector de punteros al inicio de cada uno de los argumentos del comando
	NOTA: En args[0] tiene que ir el puntero al inicio de la cadena que contiene el comando
			CLIENT *clnt: puntero a la estructura CLIENT creada con clnt_create()
Descripcion: Ejecuta el comando put en cliente ftp. Lleva un archivo al servidor
Llama a:
Fecha ultima modificacion: 30-04-09
Ultima modificacion: Mejoramos la presentacion de los mensajes al usuario
---------------------------------------------------------------------------------------------------*/
        
int ejecutar_put(char **args, CLIENT *clnt) {
	int bytes; //  Bytes transferidos del archivo remoto al local
	
	// Variables para manejar las RPC
	struct sNMod  abrefichero_rmt_1_arg;
	struct sPut  escribefichero_rmt_1_arg;
	struct sFich  *result_1;
	char * *result_2;
	char * *result_3;
	int  close_rmt_1_arg;

	// Variables para manejar el archivo local
	int fd;		// Descriptor del archivo abierto localmente
	int nread; 	//  Variable donde almacenamos lo leido del archivo local
	
	// Comprobamos que hay un numero correcto de argumentos
	if(args [1]==NULL) {
		printf ("Numero de argumentos invalido\n");
		return (-1);
	}
	
	// Si no se ha especificado un nombre de archivo para el remoto
	// se supone que se quiere nombrar como el local
	if (args[2] == NULL )
		args[2] = args[1];

	// Nombre del archivo que queremos abrir en el servidor remoto
	strcpy(abrefichero_rmt_1_arg.nombre, args[2]);
	// Modo de solo escritura y si no existe lo creamos
	abrefichero_rmt_1_arg.modo = O_WRONLY | O_CREAT;

	// Abrimos el fichero remoto
	result_1 = abrefichero_rmt_1(&abrefichero_rmt_1_arg, clnt);
	if (result_1 == (struct sFich *) NULL) {
		clnt_perror(clnt, "call failed");
	}
	printf("%s",result_1->replica);
	// Obtenemos replica, esperamos 2 (correcto)
	if (ControlReplicas(result_1->replica) != 2) {
		return (-1);
	}	

	// Abrimos fichero local
	if ( (fd = open(args[1], O_RDONLY)) < 0) {
		printf("Error al abrir el archivo local %s\n", args[1]);
		perror("miftprpc");
		return -1;
	}
	
	escribefichero_rmt_1_arg.descriptor = result_1->descriptor;
	// Leemos del fichero local y lo leido lo escribimos en el fichero remoto
	// Iniciamos bytes transmitidos a cero
	bytes = 0;
	while( ( nread = read(fd, escribefichero_rmt_1_arg.buffer, TAM_BUFFER)) != 0)	{
		if( nread < 0)	{
			printf("Error en la lectura del archivo %s\n",args[1]);
			perror("mitfprpc");
			return -1;
		}
		else {
			// Damos el numero de bytes. Hay que hacerlo despues de cada lectura porque puede ser que no siempre read
			// lea lo que le digamos como argumento
			escribefichero_rmt_1_arg.nbytes = nread;
			// Escribimos en el fichero remoto
			result_2 = escribefichero_rmt_1(&escribefichero_rmt_1_arg, clnt);
			if (result_2 == (char **) NULL) {
				clnt_perror(clnt, "call failed");
				exit(1);
			}
			printf("%s",*result_2);
			if (ControlReplicas(*result_2) != 2) {
				return -1;
			}
			bytes += nread;
		}
	}
	
	close_rmt_1_arg = result_1->descriptor;
	result_3 = close_rmt_1(&close_rmt_1_arg, clnt);
	if (result_3 == (char **) NULL) {
		clnt_perror(clnt, "call failed");
		exit(1);
	}
	printf("%s",*result_3);
	if (ControlReplicas(*result_3) != 2) {
		return -1;
	}
	
	// si llegamos aqui es que todo ha ido bien y devolvemos los bytes transmitidos
	return bytes;
}

/*--------------------------------------------------------------------------------------------------
Funcion: ejecutar_get()
Sintaxis: int ejecutar_get(char **args, CLIENT *clnt;)
Prototipo en: rpcftp.h
Argumentos: char **args:  vector de punteros al inicio de cada uno de los argumentos del comando
	NOTA: En args[0] tiene que ir el puntero al inicio de la cadena que contiene el comando
			CLIENT *clnt: puntero a la estructura CLIENT creada con clnt_create()
Descripcion: Ejecuta el comando get en cliente ftp. Trae un archivo al servidor
Llama a:
Fecha ultima modificacion: 30-04-09
Ultima modificacion: Mejoramos la presentacion de los mensajes al usuario
---------------------------------------------------------------------------------------------------*/

int ejecutar_get(char **args, CLIENT *clnt) {
	int bytes; // bytes transferidos del archivo remoto al local
	 // Variables para manejar las RPC
        struct sNMod  abrefichero_rmt_1_arg;
        struct sFich  *result_1;
        struct sGet   *result_2;
        char * *result_3;
        int  close_rmt_1_arg;
        
	int  rmtfd; // Descritor del archivo abierto remotamente
	
        // Variables para manejar el archivo local
        int fd;         // Descriptor del archivo abierto localmente
        int nwrite;      //  Variable donde almacenamos la cantidad de bytes escritos en el archivo local 
	
        // Comprobamos que hay un numero correcto de argumentos
        if(args [1]==NULL) {
                printf ("Numero de argumentos invalido\n");
                return (-1);
        }

        // Si no se ha especificado un nombre de archivo para el remoto
        // se supone que se quiere nombrar como el local
        if (args[2] == NULL )
                args[2] = args[1];

        // Nombre del archivo que queremos abrir en el servidor remoto
        strcpy(abrefichero_rmt_1_arg.nombre, args[1]);
        // Modo de solo lectura
        abrefichero_rmt_1_arg.modo = O_RDONLY;

        // Abrimos el fichero remoto
        result_1 = abrefichero_rmt_1(&abrefichero_rmt_1_arg, clnt);
        if (result_1 == (struct sFich *) NULL) {
                clnt_perror(clnt, "call failed");
        	exit(1);
	}
        printf("%s",result_1->replica);
        // Obtenemos replica, esperamos 2 (correcto)
        if (ControlReplicas(result_1->replica) != 2) {
                return (-1);
        }

        // Abrimos fichero local con modo de escritura y lo creamos si no existe
        if ( (fd = open(args[2], O_WRONLY | O_CREAT,  S_IRWXU | S_IRWXG | S_IRWXO)) < 0) {
                printf("Error al abrir el archivo local %s\n", args[2]);
                perror("miftprpc");
                return -1;
        }

        rmtfd = result_1->descriptor;
        // Leemos del fichero local y lo leido lo escribimos en el fichero remoto
	// Iniciamos bytes transmitidos a cero
	bytes = 0;
        while(1) {
		result_2 = leefichero_rmt_1(&rmtfd, clnt);
		if (result_2 == (struct sGet *) NULL) {
                                clnt_perror(clnt, "call failed");
                                exit(1);
                }
                if (ControlReplicas(result_2->replica) == 2) {
			printf("%s",result_2->replica);
			// Ultima escritura
			nwrite = write(fd, result_2->buffer, result_2->nbytes);
                        if (nwrite <=0) {
                                printf("Error en la escritura del archivo %s",args[2]);
                                perror("miftprpc");
                                return -1;
                        }
			bytes += nwrite;
			break;
                }
                // si el numero empieza por tenemos que volver a llama leefichero_rmt_1() porque hay
                // mas que leer en el fichero
		else if (ControlReplicas(result_2->replica) == 1) {
                        // Escribimos lo leido del archivo remoto y no salimos del bucle porque 
			// la replica esta indicando que todavia no hemos acabado
                        nwrite = write(fd, result_2->buffer, result_2->nbytes);
                        if (nwrite <=0) {
                                printf("Error en la escritura del archivo %s",args[2]);
                                perror("miftprpc");
                                return -1;
                        }
                        bytes += nwrite;
                }
		else {
			printf("%s",result_2->replica);
			return -1;
		}
        }

        close_rmt_1_arg = result_1->descriptor;
        result_3 = close_rmt_1(&close_rmt_1_arg, clnt);
        if (result_3 == (char **) NULL) {
                clnt_perror(clnt, "call failed");
                exit(1);
        }
        printf("%s",*result_3);
        if (ControlReplicas(*result_3) != 2) {
                return -1;
        }
	// Si se llega aqui es que todo ha sido correcto
	return bytes;
}

/*--------------------------------------------------------------------------------------------------
Funcion: ejecutar_cd()
Sintaxis: int ejecutar_cd(char **args, CLIENT *clnt;)
Prototipo en: rpcftp.h
Argumentos: char **args:  vector de punteros al inicio de cada uno de los argumentos del comando
	NOTA: En args[0] tiene que ir el puntero al inicio de la cadena que contiene el comando
			CLIENT *clnt: puntero a la estructura CLIENT creada con clnt_create()
Descripcion: Ejecuta el comando cd en cliente ftp. Cambia el directorio de trabajo del servidor
Llama a:
Fecha ultima modificacion: 30-04-09
Ultima modificacion: Desarrollo de la funcion
---------------------------------------------------------------------------------------------------*/

void ejecutar_cd(char **args, CLIENT *clnt) {
	char **result;
	if(args [1]==NULL)      { 
               printf ("Numero de argumentos invalido\n");
               return;
       	}
	result = cd_rmt_1(args+1, clnt);
        if (result == (char **) NULL) {
                clnt_perror(clnt, "call failed");
                exit(1);
        }
	if (*result == NULL ) {
                exit(1);
        }
	// Sea cual sea la replica actuamos de la misma manera       
	printf("%s",*result);
        return;

}

/*--------------------------------------------------------------------------------------------------
Funcion: ejecutar_rm()
Sintaxis: int ejecutar_get(char **args, CLIENT *clnt;)
Prototipo en: rpcftp.h
Argumentos: char **args:  vector de punteros al inicio de cada uno de los argumentos del comando
	NOTA: En args[0] tiene que ir el puntero al inicio de la cadena que contiene el comando
			CLIENT *clnt: puntero a la estructura CLIENT creada con clnt_create()
Descripcion: Ejecuta el comando rm en cliente ftp. Borra un archivo en el servidor
Llama a:
Fecha ultima modificacion: 30-04-09
Ultima modificacion: Desarrollo de la funcion
---------------------------------------------------------------------------------------------------*/

void ejecutar_rm(char **args, CLIENT *clnt) {
	char * *result;

	if (args[1] == NULL) {
		fprintf(stdout,"Número insuficiente de argumentos\n");
		return;
	}
	result = rm_rmt_1(args+1, clnt);
        if (result == (char **) NULL) {
                clnt_perror(clnt, "call failed");
        }
	if (*result == NULL ) {
		fprintf(stderr,"Error en el mensaje recibido del servidor\n");
		exit(1);
	}
        printf("%s",*result);
        return;

}

/*--------------------------------------------------------------------------------------------------
Funcion: ejecutar_list()
Sintaxis: int ejecutar_list(char **args, CLIENT *clnt;)
Prototipo en: rpcftp.h
Argumentos: char **args:  vector de punteros al inicio de cada uno de los argumentos del comando
	NOTA: En args[0] tiene que ir el puntero al inicio de la cadena que contiene el comando
			CLIENT *clnt: puntero a la estructura CLIENT creada con clnt_create()
Descripcion: Ejecuta el comando list en cliente ftp. Lista un directorio del servidor
Llama a:
Fecha ultima modificacion: 30-04-09
Ultima modificacion: Desarrollo de la funcion
---------------------------------------------------------------------------------------------------*/

int ejecutar_list(char **args, CLIENT *clnt) {
	struct sGet * result;	
	int bytes = 0;		// Bytes transmitidos para listar el directorio
	int j;			// Para controlar el bucle
	char cad[COM_MAX] = "";	// cadena  donde se almacena lo que vamos a mandar como argumento del list_rmt_1
				// Notese que no vamos a necesitar nunca mas de COM_MAX bytes porque este es el 
				// tamano maximo de la linea de comandos
	char *arglist;
	// Aqui hay problema: Necesitamos juntar otra vez la cadena del comando pero sin incluir el nombre del 
	// comando propio (list) porque list_rmt_1 hace luego la separacion. Esto es asi porque para transmitir
	// todos los argumentos lo tenemos que haces a traves de una unica cadena ya que estamos llamando a un 
	// procedimiento remoto
	// Otra solucion posible podria ser separar el caso en el que tenemos que ejecutar ejecutar_list() y no 
	// no separar la linea de comando en argumentos. Esta solucion no me gusta porque habria que quitar el nombre
	// del comando (list) porque ejecutar_list espera solo opciones y nombre del directorio. Ademas hay que anadir 
	// mas sentencias if a la funcion interfaz()

	arglist = cad;
	// Formamos arglist con los 'trozos' de la linea de comandos.
	// Empezamos con j = 1 porque no queremos incluir el nombre del comando
	for(j=1;j<MAX_ARG && args[j] !=NULL;j++) {
		// Mucho cuidado con esto. Hay que tener inicializado arglist a ""
		// Y no olvidar el espacio intermedio (Hay que separar los argumentos por espacios)
		sprintf(arglist,"%s %s",arglist,args[j]);
	}
	while(1) {
		result = list_rmt_1(&arglist, clnt);
	        if (result == (struct sGet *) NULL) {
        	        clnt_perror(clnt, "call failed");
        	}
        	if (result->replica == NULL ) {
        	        fprintf(stderr,"Error en el mensaje recibido del servidor\n");
                	exit(1);
        	}
		switch (ControlReplicas(result->replica)) {
			case 1:
				write(fileno(stdout),result->buffer,result->nbytes);
				bytes += result->nbytes;
				break;
			case 2:	
				write(fileno(stdout),result->buffer,result->nbytes);
                                bytes += result->nbytes;
                                fprintf(stdout,"%s",result->replica);	
        	        	return bytes;
			case 5:
				fprintf(stdout,"%s",result->replica);
				return -1;
			default:
				return -1;
	        }
	}
	return 0;
}
