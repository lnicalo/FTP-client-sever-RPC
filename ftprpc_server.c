/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "ftprpc.h"
#include "miftprpcs.h"
#include "librerias.h"
#include <dirent.h>	// Archivo de cabecera para las funciones que manejan directorios


char** autenticacion_rmt_1(struct sUser *argp, struct svc_req *rqstp)
{
	static char * result;
	struct spwd *password; //Password encriptada que corresponde con el nombre y recibimos del sistema
	char * passwordencriptada; //Password encriptada que corresponde a la que introduce el usuario
	char semilla[3];  //Semilla para crypt
	struct passwd * info_user; //Informacion del usuario via getpwnam
	if ((result = malloc(TAM_REPLICA)) ==  NULL ) {
		sprintf(result, "500 Error en la reserva de memoria\n%s",strerror(errno));
		return(&result);
	}
	// Comprobamos que el usuario y el password enviado son correctos
	password = (struct spwd *) getspnam ((char *)argp->login);

	if (password != NULL)	{
		sprintf (semilla, "%c%c\0", password->sp_pwdp[0], password->sp_pwdp[1]);
		passwordencriptada = (char *) (crypt ((char *)argp->passwd,semilla));
		// Comparamos las dos contrasenas y vemos si son correctas 
		if (strcmp ((char *)(password->sp_pwdp),passwordencriptada)==0) { //Contraseña correcta
			sprintf (result, "200 Login Correcto\n");
			info_user = (struct passwd *) getpwnam ((char *)argp->login);
			// Cambiamos el usuario que ejecuta el programa
			if (setuid(info_user->pw_uid) == -1) {
				sprintf(result, "500 %s\n",strerror(errno));
				return(&result);
			}
			if (setgid(info_user->pw_gid) == -1) {
				sprintf(result, "500 %s\n",strerror(errno));
				return(&result);
			}
			// Le llevamos a su directorio HOME
			chdir ((char *) info_user->pw_dir);			
		}
		else {	//Contraseña Incorrecta
			sprintf (result, "500 Login Incorrecto\n");
		}
	}
	else {
		sprintf (result, "500 Login Incorrecto\n");
	}
	return (&result);
}

struct sFich* abrefichero_rmt_1(struct sNMod *argp, struct svc_req *rqstp)
{
	char **sys_errlist;
	int fd; // Descriptor de archivo
	static struct sFich  result;

	// Abrimos el fichero con el nombre y modo especificado
	if ( (fd = open(argp->nombre, argp->modo, S_IRWXU | S_IRWXG | S_IRWXO )) < 0 ) {
		sprintf(result.replica,"500 Error al abrir %s\n%s\n",argp->nombre,strerror(errno));

	}
	else {
		sprintf(result.replica,"200 Abierto %s\n",argp->nombre);
	}
	
	result.descriptor = fd;
	return ((struct sFich*)&result);
}

struct sGet* leefichero_rmt_1(int *argp, struct svc_req *rqstp)
{
	static struct sGet result;
	static int primera = 1; // Indica si es la primera vez que se ejecuta esta funcion o no
	static int bytestotal = 0; // Indica la suma de bytes leidos del archivo
	char *ptr;
	int n;

        ptr = result.buffer;
        n = read(*argp, result.buffer, TAM_BUFFER);
	result.nbytes = n;
	bytestotal += n;
        if(n < 0) {
		sprintf(result.replica, "500 Error de read()\n%s\n",strerror(errno));
		// Hay que reiniciar el valor de la variable
		bytestotal = 0;
                return ((struct sGet *)&result);
        }
	else if (n < TAM_BUFFER || n == 0) {
		// Hay que reiniciar el valor de la variable bytestotal porque
		// Si no se suman con los bytes transmitidos para todos los archivos 
		// y no en cada archivo
	        sprintf(result.replica,"200 Todo leido total %d\n",bytestotal);
		bytestotal = 0;
		return ((struct sGet*)&result);		
	}
	else {
        	sprintf(result.replica,"150 Leyendo el contenido del fichero... %d bytes\n",n);
		return ((struct sGet*)&result);
	}
}

char ** escribefichero_rmt_1(struct sPut *argp,	struct svc_req *rqstp)
{
	static char *result;
	static int primera = 1; // Indica si es la primera vez que se ejecuta esta funcion o no
	if (primera == 1) {
		if ((result = (char*) malloc(TAM_REPLICA)) == NULL) {
			sprintf(result, "500 Error en la reserva de memoria\n%s\n",strerror(errno));
                	return(&result);
		}
		primera = 0;
	}
	int nLeft;
	int n;
	char *ptr;
	
	n = 0;
	ptr = argp->buffer;
	nLeft = argp->nbytes;
	while ( n < nLeft ) {
		ptr += n;
		nLeft -=n;
		n = write(argp->descriptor, argp->buffer, nLeft);
		if(n <= 0) {
			sprintf(result, "500 Error en la reserva de memoria\n%s\n",strerror(errno));
                	return(&result);
		}
	}
	
	sprintf(result,"200 Escrito %d\n",n);
	return ((char **)&result);
}

char ** close_rmt_1(int *argp, struct svc_req *rqstp)
{
	static char *result;
	static int  primera = 1; // Bandera que indica que es la primera vez que se ejecuta la funcion
	if (primera == 1) { 
                if ((result = (char*) malloc(TAM_REPLICA)) == NULL) {
			sprintf(result, "500 Error en la reserva de memoria\n%s\n",strerror(errno));
                	return(&result);
                }
                primera = 0;
        }

	if (close(*argp) < 0 ) {
		sprintf(result,"500 Error al cerrar el archivo\n%s\n",strerror(errno));
		return ((char **)&result);
	}
	
	sprintf(result,"200 Archivo cerrado\n");
	return ((char **)&result);
}

char ** cd_rmt_1(char ** path) {
	static char *result;
	static int primera = 1; 
	if (primera == 1) {
                if ((result = (char*) malloc(TAM_REPLICA)) == NULL) {
			sprintf(result,"500 Error al cerrar el archivo\n%s\n",strerror(errno));
	                return ((char **)&result);
                }
                primera = 0;
        }

	if (chdir(*path) == -1) {
		sprintf(result,"550 Error al intentar cambiar %s directorio.\n",*path,strerror(errno));
	}
	else {
		sprintf(result,"250 Cambio de directorio a %s\n",*path);
	}
	return ((char **)&result);
}

char ** rm_rmt_1(char **argp, struct svc_req *rqstp) {
	static char * result;
	static int primera = 1;
        if (primera == 1) {
                if ((result = (char*) malloc(TAM_REPLICA)) == NULL) {
			sprintf(result,"500 Error al cerrar el archivo\n%s\n",strerror(errno));
                	return ((char **)&result);
                }
                primera = 0;
        }
	if (unlink(*argp) == -1) {
                sprintf(result,"550 Error al intentar borrar el archivo%s\n%s\n",*argp,strerror(errno));
        }
        else {  
                sprintf(result,"250 Borrado %s\n",*argp);
        }     
        return (&result);
}

struct sGet* list_rmt_1(char **argp, struct svc_req *rqstp) {
	static struct sGet  result;
	char cwd[256];
	static char path[256];
	DIR *dirp;
	static DIR *dirp2;
	static struct dirent *dp;
	static int primera = 1;
	// Estructura que mantiene las opciones que hay activas
	struct sOpciones {
		unsigned ocultos;
	};
	static struct sOpciones op = {0}; // Estructura que mantiene las opciones entre llamadas a la funcion
	// Variables utiles para separar la linea de comandos para posteriormente detectar si hay alguna opcion
	char *saveptr;          
        char *token[MAX_ARG];   // Vector de punteros a los distintos argumentos de los comandos
        int j;                  // Variable de control para el bucle que se hace en la separacion de los argumentos del comando 
	
	// Notese, y esto es muy importante, que no conviene trabajar directamente con los argumentos de la funcion
        // no vaya ser que el cliente los cambie entre las sucesivas llamadas pudiendose generar muchos problemas
        // Lo conveniente es en la primera llamada a list_rmt_1() guardar los datos procedentes del cliente (argumentos)
        // en variables estaticas para que luego si son necesarios sucesivas llamadas a esta funcion para leer el directorio completamente sin
        // que haya problemas si el cliente le da por cambiar argp. Si asi fuera lo obviamos y seguimos leyendo el directorio normalmente
	// Una vez completada la lectura del directorio tenemos en cuenta el argumento (cambio de la variable primera a 0)

	if (primera == 1) {
		// Reiniciamos las opciones. Esto es importante porque si no lo hacemos puede que se conserven 
		// las de llamadas anteriores
		op.ocultos = 0;
		
		// De momento hacemos que se liste el directorio de trabajo. Si el usuario especifico otro lo cambiamos
		// despues. Esto lo hago asi porque me ahorra comprobaciones.
                getcwd(cwd,255);
                // Ponemos el final de cadena
                cwd[255] = 0;
                // Ponemos como path el directorio de trabajo
                strcpy(path,cwd);

		// Si contine algo lo separamos en argumentos por medio de los espacios,buscamos opciones y directorio a listar
		if (strlen(*argp)) {
			// Separamos en argumentos
			j=0;
                	if ((token[j] = (char *)strtok_r(*argp, " ", &saveptr)) != NULL) {
                        	j++;
                        	while (((token[j] = (char *)strtok_r(NULL, " ", &saveptr)) != NULL) &&  (j < MAX_ARG))  {
                                	j++;
					// Si se llega al limite de argumentos salimos del bucle y devolvemos error.
                                	if (j == MAX_ARG) {
						sprintf(result.replica,"500 Demasiados argumentos\n");
						return(&result);
                                	}
                        	}
                	}
			// Vemos si en los argumentos, a los que apunta el vector de punteros token, tiene opciones
                	for(j=0;j<MAX_ARG && token[j]!=NULL;j++) {
                        	// Si empieza por guion es que es una opcion
                        	if(token[j][0] == '-') {
					// Lo hacemos con un switch aunque si puede con un if. Pero se espera que en proximas
					// mejoras se anadan mas opciones
                                	switch(token[j][1]) {
                                	case 'a':
                                        	// Se desea que se impriman los archivos y directorios que empiezan por .
                                        	op.ocultos = 1;
                       			}
               			}
                	}
			// Buscamos el directorio que se quiere listar
			for (j=0;j<MAX_ARG && token[j]!=NULL;j++) {
				if(token[j][0] != '-') {
					// Si encontramos un directorio lo ponemos como path para listar. Cambiando el
					// directorio de trabajo que habiamos puesto por defecto.
					strcpy(path,token[j]);
					break;	
				}
			}
		}

                if ( (dirp = opendir(path)) == NULL) {
                        sprintf(result.replica,"550 %s: Error al intentar acceder al directorio\n%s\n",*argp,strerror(errno));
                        return (&result);
                }
		// Creamos dirp2 que es la estructura DIR que va a mantener la informacion entre llamadas
		// No se por que pero si lo haces con dirp da problemas
                if ((dirp2 = malloc(sizeof(DIR))) == NULL ) {
			sprintf(result.replica,"500 Error al cerrar el archivo\n%s\n",strerror(errno));
                        return (&result);
                }
                bcopy(dirp,dirp2,sizeof(DIR));
                primera = 0;
	}
	
	// Leemos las entradas del directorio
        while ((dp = readdir(dirp2)) != NULL) {
                // Saltamos las referencias al directorio actual y al directorio padre
                if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
                        continue;
                else {
			// Si esta marcada de opcion de mostrar ocultos mostramos todos
			// Si no lleva punto al comienzo lo mostramos tambien sea cual sea el estado de la opcion ocultos
			if (op.ocultos == 1 || *(dp->d_name) != '.') {
                        	sprintf(result.buffer,"%s\n",dp->d_name);
                        	result.nbytes = strlen(dp->d_name)+1;
                        	sprintf(result.replica,"150 Transmitiendo el contenido del directorio\n");
                        	return(&result);
			}
                }
        }

	sprintf(result.buffer,"\n");
	sprintf(result.replica,"250 Completdo el listado de %s\n",path);
	result.nbytes = 1;
	primera = 1;
        return (&result);
}
