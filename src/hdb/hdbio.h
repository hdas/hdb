#define __hdbio_h

#ifndef __db_h
#include "db.h"
#endif

#ifndef __token_h
#include "token.h"
#endif

#ifndef __expr_h
#include "expr.h"
#endif

#ifndef __where_h
#include "where.h"
#endif


#ifdef OS_WINDOWS
#include <sys/types.h>
#include <stdio.h>
#include <winsock.h>
#include <windows.h> // thread
#include <process.h> // thread
#include <stddef.h>
#else // Linux
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h> /* close */
#endif


#include "DResult.h"


#define SERVER_PORT 1204
#define MAX_RCVBUF  256



int hdb_server();
void _cdecl hdb_doio(void * pArgSd);
int hdb_process_request(DSession * ssn, char * req_buf);
int dummy_process_request(DSession * pSsn, char * req_buf); // just a dummy for a test insert into stmt
int hdb_output_result(int hSock, DResult * result);
int hdb_sout(int hSock, char * data);


