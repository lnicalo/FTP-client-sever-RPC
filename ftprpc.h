/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _FTPRPC_H_RPCGEN
#define	_FTPRPC_H_RPCGEN

#include <rpc/rpc.h>

struct sUser {
	char login[256];
	char passwd[256];
};
typedef struct sUser sUser;

struct sNMod {
	char nombre[256];
	int modo;
};
typedef struct sNMod sNMod;

struct sFich {
	char replica[1024];
	int descriptor;
};
typedef struct sFich sFich;

struct sGet {
	char replica[1024];
	char buffer[1024];
	int nbytes;
};
typedef struct sGet sGet;

struct sPut {
	int descriptor;
	char buffer[1024];
	int nbytes;
};
typedef struct sPut sPut;

#define	MI_FTP_RPC	0x20000002
#define	MI_FTP_RPC_VERS	1
#define	AUTENTICACION_RMT	1
extern  char ** autenticacion_rmt_1();
#define	ABREFICHERO_RMT	2
extern  struct sFich * abrefichero_rmt_1();
#define	LEEFICHERO_RMT	3
extern  struct sGet * leefichero_rmt_1();
#define	ESCRIBEFICHERO_RMT	4
extern  char ** escribefichero_rmt_1();
#define	CLOSE_RMT	5
extern  char ** close_rmt_1();
#define	CD_RMT	6
extern  char ** cd_rmt_1();
#define	RM_RMT	7
extern  char ** rm_rmt_1();
#define	LIST_RMT	8
extern  struct sGet * list_rmt_1();
extern int mi_ftp_rpc_1_freeresult();

/* the xdr functions */
extern bool_t xdr_sUser();
extern bool_t xdr_sNMod();
extern bool_t xdr_sFich();
extern bool_t xdr_sGet();
extern bool_t xdr_sPut();

#endif /* !_FTPRPC_H_RPCGEN */
