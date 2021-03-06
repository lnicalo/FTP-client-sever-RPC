/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "ftprpc.h"
#ifndef _KERNEL
#include <stdio.h>
#include <stdlib.h> /* getenv, exit */
#endif /* !_KERNEL */

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

char **
autenticacion_rmt_1(argp, clnt)
	struct sUser *argp;
	CLIENT *clnt;
{
	static char *clnt_res;

	memset((char *)&clnt_res, 0, sizeof (clnt_res));
	if (clnt_call(clnt, AUTENTICACION_RMT,
		(xdrproc_t) xdr_sUser, (caddr_t) argp,
		(xdrproc_t) xdr_wrapstring, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

struct sFich *
abrefichero_rmt_1(argp, clnt)
	struct sNMod *argp;
	CLIENT *clnt;
{
	static struct sFich clnt_res;

	memset((char *)&clnt_res, 0, sizeof (clnt_res));
	if (clnt_call(clnt, ABREFICHERO_RMT,
		(xdrproc_t) xdr_sNMod, (caddr_t) argp,
		(xdrproc_t) xdr_sFich, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

struct sGet *
leefichero_rmt_1(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static struct sGet clnt_res;

	memset((char *)&clnt_res, 0, sizeof (clnt_res));
	if (clnt_call(clnt, LEEFICHERO_RMT,
		(xdrproc_t) xdr_int, (caddr_t) argp,
		(xdrproc_t) xdr_sGet, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

char **
escribefichero_rmt_1(argp, clnt)
	struct sPut *argp;
	CLIENT *clnt;
{
	static char *clnt_res;

	memset((char *)&clnt_res, 0, sizeof (clnt_res));
	if (clnt_call(clnt, ESCRIBEFICHERO_RMT,
		(xdrproc_t) xdr_sPut, (caddr_t) argp,
		(xdrproc_t) xdr_wrapstring, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

char **
close_rmt_1(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static char *clnt_res;

	memset((char *)&clnt_res, 0, sizeof (clnt_res));
	if (clnt_call(clnt, CLOSE_RMT,
		(xdrproc_t) xdr_int, (caddr_t) argp,
		(xdrproc_t) xdr_wrapstring, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

char **
cd_rmt_1(argp, clnt)
	char **argp;
	CLIENT *clnt;
{
	static char *clnt_res;

	memset((char *)&clnt_res, 0, sizeof (clnt_res));
	if (clnt_call(clnt, CD_RMT,
		(xdrproc_t) xdr_wrapstring, (caddr_t) argp,
		(xdrproc_t) xdr_wrapstring, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

char **
rm_rmt_1(argp, clnt)
	char **argp;
	CLIENT *clnt;
{
	static char *clnt_res;

	memset((char *)&clnt_res, 0, sizeof (clnt_res));
	if (clnt_call(clnt, RM_RMT,
		(xdrproc_t) xdr_wrapstring, (caddr_t) argp,
		(xdrproc_t) xdr_wrapstring, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

struct sGet *
list_rmt_1(argp, clnt)
	char **argp;
	CLIENT *clnt;
{
	static struct sGet clnt_res;

	memset((char *)&clnt_res, 0, sizeof (clnt_res));
	if (clnt_call(clnt, LIST_RMT,
		(xdrproc_t) xdr_wrapstring, (caddr_t) argp,
		(xdrproc_t) xdr_sGet, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}
