#define __dbclnt_h

#ifndef __db_h
#include "../hdb/db.h"
#endif

#ifndef __token_h
#include "..\hdb\token.h"
#endif
#include "..\hdb\DResult.h"
#include "..\hdb\DRow.h"


#ifdef OS_WINDOWS
#include <sys/types.h>
#include <stdio.h>
#include <winsock.h>
#include <windows.h>
#include <process.h>
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

#define SERVER_PORT 1204
#define MAX_RCVBUF  4096

int hdb_connect(char * hostname);

int hdb_send_sql(int hSock, char * sql);
DResult * hdb_recv_result(int hSock);
int hdb_sout(int hSock, char * data);



