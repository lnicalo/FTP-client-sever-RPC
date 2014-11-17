/*******************************************************
rpc.x fichero de especificaciones RPC para la aplicacion FTP

Se definen los siguientes procedimientos:
        string Autenticacion_rmt_1(struct sUser)
        struct sFich AbreFichero_rmt_2(struct sNMod)
        struct sGet LeeFichero_rmt_3(int fd)
        string EscribeFichero_rmt_4(struct sPut)
        string close_rmt_5(int fd)
*************************************************************/

#define CAD_MAX 	256 	/* numero bytes reservados para almacanar el login y el password */
#define TAM_BUFFER 	1024  /*Tamaño del buffer*/
#define TAM_REPLICA	1024


struct sUser {
        char login[CAD_MAX];
        char passwd[CAD_MAX];
};

struct sNMod {
        char nombre[CAD_MAX];
        int  modo;
};

struct sFich {
        char replica[TAM_REPLICA];
        int descriptor;
};

struct sGet {
        char replica[TAM_REPLICA];
        char buffer[TAM_BUFFER];
        int  nbytes;
};

struct sPut {
        int  descriptor;
        char buffer[TAM_BUFFER];
        int  nbytes;
};

program MI_FTP_RPC
{
        version MI_FTP_RPC_VERS
        {
                /* declaracion de los prodedimientos remotos */
                string AUTENTICACION_RMT(struct sUser) = 1;
                struct sFich ABREFICHERO_RMT(struct sNMod) = 2;
                struct sGet  LEEFICHERO_RMT(int fd) = 3;
                string ESCRIBEFICHERO_RMT(struct sPut) = 4;
                string CLOSE_RMT(int fd) = 5;
		string CD_RMT(string path) = 6;
		string RM_RMT(string fich) = 7;
		struct sGet  LIST_RMT(string path) = 8;
        } = 1;
} = 0x20000002; /* Numero de programa */
