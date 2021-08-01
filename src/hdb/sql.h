#define __sql_h

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


#include "DSession.h"



int SQL();
int RunFile(DSession * ssn, char * sql);


DResult * ExecuteSql(DSession * pSsn, char * sql);

DResult * SQL_CREATE_TABLE(DSession * ssn, DQuery * qry);
DResult * SQL_DROP_TABLE(DSession * ssn, DQuery * qry);
DResult * SQL_CREATE_INDEX(DSession * ssn, DQuery * qry);
DResult * SQL_CREATE_VIEW(DSession * ssn, DQuery * qry);
DResult * SQL_INSERT_INTO(DSession * ssn, DQuery * qry);
DResult * SQL_UPDATE(DSession * ssn, DQuery * qry);
DResult * SQL_ALTER_TABLE(DSession * ssn, DQuery * qry);
DResult * SQL_DELETE(DSession * ssn, DQuery * qry);
DResult * SQL_DESC(DSession * ssn, DQuery * qry);

int CheckConstraints(DTable * tbl, int nTbl);
int GetErrMsg(int errcd, char * errmsg);






