#ifndef __db_h
#include "db.h"
#endif


DBGlobal global_opt;

/*DBSession * dCreateSession(char * username)
{
DBSession * ssn=NULL;
int i;

ssn = (DBSession*)malloc(sizeof(DBSession));
ssn->display = TRUE;
ssn->hSock = -1;
ssn->table_lockid = 1;
strcpy(ssn->username, username);
for(i=0; i<MAX_CACHE_TABLE; i++) ssn->pCacheTable[i]=NULL;

return ssn;
}*/

/*int dCloseSession(DBSession * ssn)
{
int i;

if(ssn==NULL) return true;

for(i=0; i<MAX_CACHE_TABLE; i++)
{
if(ssn->pCacheTable[i]!=NULL)
{
if(dCloseTable(ssn->pCacheTable[i])) ssn->pCacheTable[i] = NULL;
}
}
free(ssn);
return true;
}*/

/*char * dCreateStrVal(DBVariable * var)
{
char * str;
HDB_ASSERT(var!=NULL);

HDB_ASSERT( ! (var->VarClass != VCLS_VALUE && var->VarClass != VCLS_FIELD));
//if(var->VarClass != VCLS_VALUE && var->VarClass != VCLS_FIELD ) return NULL;

//chop39(var->strval);
//chop_lzero(var->strval);


if(var->DataType==DT_NUMBER)
{
str = (char*)malloc(sizeof(char) * 20);
sprintf(str, "%f", var->numval);
chop_lzero(str);
}
else if(var->DataType==DT_BOOLEAN)
{
str = (char*)malloc(sizeof(char) * 2);
if(var->numval) strcpy(str, "T"); else strcpy(str, "F");
}
else if(var->DataType==DT_CHAR || var->DataType==DT_VARCHAR)
{
str = (char*)malloc(sizeof(char) * (strlen(var->strval)+1));
strcpy(str, var->strval);
}
else if(var->DataType==DT_DATETIME)
{
str = (char*)malloc(sizeof(char) * 12);
HDB_ASSERT(strlen(var->strval) < 10);
strcpy(str, var->strval);
}
else
{
str = (char*)malloc(sizeof(char) * (strlen(var->strval)+2));
strcpy(str, var->strval);
strcat(str, "*");
}


return str;
}*/

/*DBVariable * dCreateVariable(int * pRetcd) //int DataType, int szIfStr, int * retcd)
{
DBVariable * dbv = (DBVariable*)malloc(sizeof(DBVariable));
memset((void*)dbv, 0, sizeof(DBVariable));

HDB_ASSERT(dbv!=NULL);

dbv->DataType = DT_UNKNOWN;
dbv->strval = NULL;
dbv->VarClass = VCLS_VALUE;
dbv->numval = 0;

*pRetcd = true;
return dbv;
}*/

//int dChangeDatatype(DBVariable * pVar, int new_datatype)
/*int dChangeVariableDatatype(DBVariable * pVar, int NewDatatype)
{
//if(pVar->DataType != DT_UNKNOWN) return false;
//HDB_ASSERT(pVar->DataType == DT_UNKNOWN);
if(pVar->strval == NULL) return FALSE;

if(NewDatatype==DT_NUMBER)
{
pVar->numval = atof(pVar->strval);
free(pVar->strval);
pVar->strval=NULL;
}
else if(NewDatatype==DT_BOOLEAN)
{
if(EQUAL(pVar->strval, "1") || EQUAL(pVar->strval, "T") || EQUAL(pVar->strval, "TRUE"))
pVar->numval = true;
else
pVar->numval = false;
free(pVar->strval);
pVar->strval=NULL;
}
else if((NewDatatype==DT_CHAR || NewDatatype==DT_VARCHAR))
{
chop39(pVar->strval);
}

pVar->DataType = NewDatatype;

return true;
}*/

/*int dStoreVariable(DBVariable * pVar, int DataType, char * strval)
{
if(pVar->strval!=NULL) free(pVar->strval);
pVar->strval = (char*)malloc(strlen(strval)+1);
strcpy(pVar->strval, strval);
pVar->DataType = DataType;
return true;
}*/

/*DBVariable * dCreateDuplicateVariable(DBVariable * pSrcVar, int * retcd)
{
DBVariable * tvar;
tvar = (DBVariable*)malloc(sizeof(DBVariable));
memcpy((void*)tvar, (void*)pSrcVar, sizeof(DBVariable));
if(pSrcVar->strval != NULL)
{
tvar->strval=(char*)malloc(strlen(pSrcVar->strval)+1);
strcpy(tvar->strval, pSrcVar->strval);
}
else tvar->strval = NULL;
*retcd = true;
return tvar;
}*/

/*int dDestroyVariable(DBVariable * pVar)
{
if(pVar!=NULL)
{
if(pVar->strval!=NULL) free(pVar->strval);
free(pVar);
}
return true;
}*/



/*DBStack * dCreateStack(int * retcd)
{
DBStack * stack;
stack = (DBStack*)malloc(sizeof(DBStack));
if(stack==NULL) {*retcd = ERR_NOMEM; return NULL;}
stack->link=NULL;
stack->pVar=NULL;
return stack;
}*/

/*int dPushStack(DBStack * top, DBVariable * pVar)
{
int res;
DBStack * alist;
alist = (DBStack *)malloc(sizeof(DBStack));

alist->pVar = dCreateDuplicateVariable(pVar, &res);
alist->pVar->used++;

alist->link = top->link;
top->link = alist;
return res;
}


DBVariable * dPopStack(DBStack * top, int * retcd)
{
DBVariable * pVar = NULL;
DBStack * alist;
if(top->link == NULL) { *retcd = ERR_EOF; return NULL; }
pVar = top->link->pVar;
alist = top->link;
top->link = top->link->link;
free(alist);
*retcd = true;
pVar->used--;
return pVar;
}*/

/*int dDestroyStack(DBStack * stack)
{
int retcd;
DBVariable * pVar = NULL;

if(stack==NULL) return true;

pVar = dPopStack(stack, &retcd);
while(retcd==true)
{
dDestroyVariable(pVar);
pVar = dPopStack(stack, &retcd);
}
free(stack);
return true;
}*/

int icp(int oprtr)
{
	switch (oprtr)
	{
	case OP_CAT: return 7;
	case OP_UMINUS: return 7;
	case OP_UPLUS: return 7;
	case OP_POW: return 7;

	case OP_MULT: return 5;
	case OP_DIVI: return 5;
	case OP_DIV: return 5;
	case OP_MOD: return 5;


	case OP_PLUS: return 4;
	case OP_MINUS: return 4;

	case OP_NOT: return 3;

	case OP_EQ: return 2;
	case OP_GT: return 2;
	case OP_LT: return 2;
	case OP_GE: return 2;
	case OP_LE: return 2;
	case OP_NE: return 2;

	case OP_AND: return 1;
	case OP_OR: return 1;

	case OP_BRACSTART: return 7;
	}
	return 10;
}

int isp(int oprtr)
{
	switch (oprtr)
	{
	case OP_CAT: return 6;
	case OP_UMINUS: return 6;
	case OP_UPLUS: return 6;
	case OP_POW: return 6;

	case OP_MULT: return 5;
	case OP_DIVI: return 5;
	case OP_DIV: return 5;
	case OP_MOD: return 5;

	case OP_PLUS: return 4;
	case OP_MINUS: return 4;

	case OP_NOT: return 3;

	case OP_EQ: return 2;
	case OP_GT: return 2;
	case OP_LT: return 2;
	case OP_GE: return 2;
	case OP_LE: return 2;
	case OP_NE: return 2;

	case OP_AND: return 1;
	case OP_OR: return 1;

	case OP_BRACSTART: return 0;
	}
	return 10;
}




int rpad(char * str, int len)
{
	int i, l = strlen(str);
	str[len] = 0x00;
	for (i = l; i < len; i++) str[i] = 0x20;
	return true;
}

int rtrim(char * str)
{
	int i, l = strlen(str);
	for (i = l - 1; i >= 0; i--)
	{
		if (str[i] == 0x20) str[i] = 0x00; else break;
	}
	return true;
}

char dGetReserveWordCode(char * rw_str) // implemented
{
	if (EQUAL(rw_str, "INSERT"))  return RW_INSERT;
	else if (EQUAL(rw_str, "UPDATE"))  return RW_UPDATE;
	else if (EQUAL(rw_str, "CREATE")) return RW_CREATE;
	else if (EQUAL(rw_str, "TABLE")) return  RW_TABLE;
	else if (EQUAL(rw_str, "FROM"))  return RW_FROM;
	else if (EQUAL(rw_str, "BY"))  return RW_BY;
	else if (EQUAL(rw_str, "SET"))  return RW_SET;
	else if (EQUAL(rw_str, "DESC"))  return RW_DESC;
	else if (EQUAL(rw_str, "DELETE"))  return RW_DELETE;
	else if (EQUAL(rw_str, "ORDER"))  return RW_ORDER;
	else if (EQUAL(rw_str, "GROUP"))  return RW_GROUP;
	else if (EQUAL(rw_str, "WHERE"))  return RW_WHERE;
	else if (EQUAL(rw_str, "SELECT"))  return RW_SELECT;
	else if (EQUAL(rw_str, "ALTER"))  return RW_ALTER;
	else if (EQUAL(rw_str, "VIEW"))  return RW_VIEW;
	else if (EQUAL(rw_str, "FUNCTION"))  return RW_FUNCTION;
	else if (EQUAL(rw_str, "ASC"))  return RW_ASC;
	else if (EQUAL(rw_str, "LIMIT"))  return RW_LIMIT;
	else if (EQUAL(rw_str, "OFFSET"))  return RW_OFFSET;
	else if (EQUAL(rw_str, "INDEX"))  return RW_INDEX;
	else if (EQUAL(rw_str, "PRIMARY"))  return RW_PRIMARY;
	else if (EQUAL(rw_str, "KEY"))  return RW_KEY;
	else if (EQUAL(rw_str, "FOREIGN"))  return RW_FOREIGN;
	else if (EQUAL(rw_str, "USER"))  return RW_USER;
	else if (EQUAL(rw_str, "GRANT"))  return RW_GRANT;
	else if (EQUAL(rw_str, "COMMIT"))  return RW_COMMIT;
	else if (EQUAL(rw_str, "ROLLBACK"))  return RW_ROLLBACK;
	else if (EQUAL(rw_str, "REVOKE"))  return RW_REVOKE;
	else if (EQUAL(rw_str, "DATABASE"))  return RW_DATABASE;
	else if (EQUAL(rw_str, "INTO"))  return RW_INTO;
	else if (EQUAL(rw_str, "AS"))  return RW_AS;
	else if (EQUAL(rw_str, "DISTINCT"))  return RW_DISTINCT;
	else if (EQUAL(rw_str, "CONSTRAINT"))  return RW_CONSTRAINT;
	else if (EQUAL(rw_str, "NULL"))  return RW_NULL;
	else if (EQUAL(rw_str, "TRUE"))  return RW_TRUE;
	else if (EQUAL(rw_str, "FALSE"))  return RW_FALSE;

	return 0; // i.e. false
}


char dGetOparatorCode(char * op_str) // implemented
{
	if (strcmp(op_str, "^") == 0) return OP_POW;
	if (strcmp(op_str, "+") == 0) return OP_PLUS;
	if (strcmp(op_str, "0+") == 0) return OP_UPLUS;
	if (strcmp(op_str, "-") == 0) return OP_MINUS;
	if (strcmp(op_str, "0-") == 0) return OP_UMINUS;
	if (strcmp(op_str, "*") == 0) return OP_MULT;
	if (strcmp(op_str, "/") == 0) return OP_DIVI;
	if (strcmp(op_str, "^") == 0) return OP_EXPN;
	if (strcmp(op_str, "(") == 0) return OP_BRACSTART;
	if (strcmp(op_str, ")") == 0) return OP_BRACSTOP;


	if (strcmp(op_str, "=") == 0) return OP_EQ;
	if (strcmp(op_str, "<>") == 0) return OP_NE;
	if (strcmp(op_str, ">") == 0) return OP_GT;
	if (strcmp(op_str, "<") == 0) return OP_LT;
	if (strcmp(op_str, ">=") == 0) return OP_GE;
	if (strcmp(op_str, "<=") == 0) return OP_LE;
	if (strcmp(op_str, "||") == 0) return OP_CAT;
	if (strcmp(op_str, "AND") == 0) return OP_AND;
	if (strcmp(op_str, "OR") == 0) return OP_OR;
	if (strcmp(op_str, "NOT") == 0) return OP_NOT;
	if (strcmp(op_str, "LIKE") == 0) return OP_LIKE;
	if (strcmp(op_str, "IN") == 0) return OP_IN;
	if (strcmp(op_str, "BETWEEN") == 0) return OP_BETWEEN;
	if (strcmp(op_str, "DIV") == 0) return OP_DIV;
	if (strcmp(op_str, "MOD") == 0) return OP_MOD;
	return 0;
}


char dGetDataTypeCode(char * dt_str) // implemented
{
	if (EQUAL(dt_str, "CHAR"))     return DT_CHAR;
	if (EQUAL(dt_str, "CHARACTER"))return DT_CHAR;
	if (EQUAL(dt_str, "VARCHAR"))  return DT_VARCHAR;
	if (EQUAL(dt_str, "VARCHAR2")) return DT_VARCHAR;
	if (EQUAL(dt_str, "NUMBER"))   return DT_NUMBER;
	if (EQUAL(dt_str, "NUMERIC"))  return DT_NUMBER;
	if (EQUAL(dt_str, "DATE"))     return DT_DATETIME;
	if (EQUAL(dt_str, "TIME"))     return DT_DATETIME;
	if (EQUAL(dt_str, "DATETIME")) return DT_DATETIME;
	if (EQUAL(dt_str, "BLOB"))     return DT_BLOB;


	return FALSE; // i.e. false ; UNKNOWN;
}

char * dGetDataTypeName(char dt_code) // implemented
{
	char * tpn = (char*)malloc(sizeof(char) * 32);
	if (tpn == NULL) return NULL;

	for (;;)
	{
		if (dt_code == DT_CHAR) { strcpy(tpn, "CHAR"); break; }
		if (dt_code == DT_VARCHAR){ strcpy(tpn, "VARCHAR"); break; }
		if (dt_code == DT_NUMBER){ strcpy(tpn, "NUMBER"); break; }
		if (dt_code == DT_DATETIME){ strcpy(tpn, "DATETIME"); break; }
		if (dt_code == DT_BLOB){ strcpy(tpn, "BLOB"); break; }
		if (dt_code == DT_BOOLEAN){ strcpy(tpn, "BOOLEAN"); break; }
		strcpy(tpn, "UNKNOWN"); break;
	}
	return tpn;
}


/*int isOparator(char * ctkn)
{
if(dGetOparatorCode(ctkn)) return true; else return false;
}*/

int isValue(char * ctkn)
{
	if (ctkn[0] == 39 || (ctkn[0] >= 48 && ctkn[0] <= 57)) return true; else return false;
}


/*int dCloseTable(DBTable * tbl)
{
time_t ltime;
struct tm * ttm;

if(tbl==NULL) return true;

time(&ltime);
ttm = localtime(&ltime);
tbl->header->last_update[0] = (char)ttm->tm_year - 100;
tbl->header->last_update[1] = (char)ttm->tm_mon + 1;
tbl->header->last_update[2] = (char)ttm->tm_mday + 1;

fseek(tbl->fp, 0, SEEK_SET);
fwrite(tbl->header, sizeof(DBHeader), 1, tbl->fp);
fclose(tbl->fp);
free(tbl->fp_buf);
free(tbl->header);
free(tbl->field);
free(tbl);
return true;
}*/

/*DBVariable * dCreateFieldVariable(char * FQ_FieldName, DBTable ** paTables, int nTables, int * pRetcd)
{
DBVariable * vv = NULL;
int res, i, tblidx=-1, fc=-1;

for(i=0; i<nTables; i++)
{
fc = dFieldCode(paTables[i], FQ_FieldName);
if(fc>=0)
{
tblidx = i;
break;
}
}

if(tblidx >= 0) // i.e. Field Matched
{
vv = dCreateVariable(&res);
vv->VarClass = VCLS_FIELD;
vv->DataType = paTables[tblidx]->field[fc].field_type;
vv->ref1 = fc;
vv->ref2 = tblidx;
*pRetcd = 1;
}
else
{
*pRetcd = ERR_BADFIELD;
}

return vv;
}*/

/*int dFieldCode(DBTable * tbl,char * FieldName)
{
int i;
for(i=0;i<tbl->nFields;i++)
{
if(strcmp(FieldName,(tbl->field+i)->field_name)==0) return i;
}
return ERR_BADFIELD;
}

int dFieldSize(DBTable * tbl,int FieldCode)
{
if(FieldCode >= tbl->nFields) return ERR_BADFIELD;
return (tbl->field + FieldCode)->field_length;
}

char dFieldType(DBTable * tbl, int FieldCode)
{
if(FieldCode >= tbl->nFields || FieldCode<0) return (char)ERR_BADFIELD;
return tbl->field[FieldCode].field_type;
}


int dFirstRecord(DBTable * tbl)
{
char flag;
tbl->abspos = sizeof(DBHeader) + tbl->nFields * sizeof(DBField) + 1;
if(fseek(tbl->fp, tbl->abspos, SEEK_SET) != 0) return ERR_POSNOTSET;
fread(&flag, 1, 1, tbl->fp);
if(feof(tbl->fp) || flag==FLAG_EOF) return ERR_EOF;
if(flag==FLAG_DELETED_RECORD) return dNextRecord(tbl);
return true;
}


int dNextRecord(DBTable * tbl)
{
char flag=FLAG_DELETED_RECORD;
while(flag==FLAG_DELETED_RECORD)
{
tbl->abspos += tbl->header->record_size;
if(fseek(tbl->fp, tbl->abspos, SEEK_SET) != 0) return ERR_POSNOTSET;
fread(&flag, 1, 1, tbl->fp);
if(feof(tbl->fp)) return ERR_EOF;
}
return true;
}

char * dFetchField(DBTable * tbl, int FieldCode, int * retcd)
{
//int i; //,offset=1; // initial value of offset is 1 because of flag byte
char * Value, * vv;

Value = (char*)malloc((tbl->field + FieldCode)->field_length + 1);
//for(i=0;i<FieldCode;i++) offset += (tbl->field +i)->field_length;
if(fseek(tbl->fp, tbl->abspos + tbl->field[FieldCode].pos_offset, SEEK_SET) != 0) { free(Value); *retcd=ERR_POSNOTSET; return NULL;}
if(fread(Value, 1, tbl->field[FieldCode].field_length, tbl->fp) != tbl->field[FieldCode].field_length ) { free(Value); *retcd=ERR_NOREAD; return NULL;}
Value[tbl->field[FieldCode].field_length]=0; // terminating character
vv = (char*)malloc(strlen(Value)+1);
strcpy(vv, Value);
free(Value);
*retcd = true;
return vv;
}

int dRestoreField(DBTable * tbl,int FieldCode,char * Value)
{
//int i,offset=1; // initial value of offset is 1 because of flag byte
char * tbuf = (char*)malloc(tbl->field[FieldCode].field_length);

if(strlen(Value) > tbl->field[FieldCode].field_length) {free(tbuf); return ERR_LARGE_VALUE; }

memset(tbuf, 0x00, tbl->field[FieldCode].field_length);

strncpy(tbuf, Value, tbl->field[FieldCode].field_length);

//for(i=0;i<FieldCode;i++) offset += (tbl->field + i)->field_length;

if(fseek(tbl->fp, tbl->abspos + tbl->field[FieldCode].pos_offset, SEEK_SET) != 0) {free(tbuf); return ERR_POSNOTSET; }

if(fwrite(tbuf, 1, tbl->field[FieldCode].field_length, tbl->fp) != tbl->field[FieldCode].field_length ) {free(tbuf); return ERR_NOWRITE; }
free(tbuf);

return true;
}

int dAppendRecord(DBTable * tbl)
{
char * buf = (char *)malloc(tbl->header->record_size);

memset(buf, 0x00, tbl->header->record_size);

buf[0]=FLAG_VALID_RECORD;

tbl->abspos = tbl->header->header_size + tbl->header->record_size * tbl->header->no_of_record;
fseek(tbl->fp, tbl->abspos, SEEK_SET);
/*fseek(tbl->fp, 0, SEEK_END);
if(tbl->abspos != ftell(tbl->fp))
{
free(buf);
return ERR_POSNOTSET;
}*/

/*if(fwrite(buf, 1, tbl->header->record_size, tbl->fp) != tbl->header->record_size ) {free(buf); return ERR_NOWRITE; }
free(buf);

tbl->header->no_of_record ++;
return true;
}*/


/*int dDeleteRecord(DBTable * tbl)
{
char flag=FLAG_DELETED_RECORD; // 0x00 means recored is deleted

if(fseek(tbl->fp, tbl->abspos, SEEK_SET) != 0) return ERR_POSNOTSET;
if(fwrite(&flag, 1, 1, tbl->fp) != 1 ) return ERR_NOWRITE;

return true;
}

int dLockTable(DBSession * ssn, DBTable * tbl)
{
HDB_ASSERT(ssn != NULL);
HDB_ASSERT(tbl != NULL);

while((tbl->Locked==true) || (tbl->LockId>0))
{
}
tbl->Locked = true;
tbl->LockId = ssn->table_lockid;
return true;
}

int dUnlockTable(DBSession * ssn, DBTable * tbl)
{
HDB_ASSERT(ssn != NULL);
HDB_ASSERT(tbl != NULL);

if(ssn->table_lockid = tbl->LockId)
{
tbl->Locked = false;
tbl->LockId = 0;
}
return true;
}

int dCreateIndex(char * IndexName, char * TableName, DBField * FieldList,unsigned short nFields, char * UserName)
{
FILE * fp;
char fname[FILE_NAME_WIDTH], fl_extn[6];
int i;
char flg=0x0D;
DBIdxHeader hdr;

hdr.record_size = 1; // initialize ; 1 for a flag byte
for(i=0; i<nFields; i++) hdr.record_size += FieldList[i].field_length;
hdr.nFields=nFields;
hdr.header_size = (unsigned short)(sizeof(DBIdxHeader) + nFields * sizeof(DBField) + 1);
strcpy(hdr.tname, TableName);
strcpy(hdr.idxname, IndexName);

strcpy(fl_extn, ".ndx");
sprintf(fname,"%s%s%s%s%s%s%s%s%s%s",HDB_BASE,DIR_SEP,VERSION,DIR_SEP,"data",DIR_SEP,UserName,DIR_SEP,IndexName, fl_extn);
fp=fopen(fname,"wb");
if(fp==NULL) return ERR_BADINDEX;

fwrite(&hdr,sizeof(DBIdxHeader),1,fp);
fwrite(FieldList,sizeof(DBField),nFields,fp);

fwrite(&flg, 1, 1, fp); // XBASE Compatible
fclose(fp);
return true;
}


// same as int  DBCreateTable()
int dCreateTable(char * TableName,DBField * FieldList,unsigned short nFields,char * UserName)
{
FILE * fp;
char fname[FILE_NAME_WIDTH], fl_extn[6];
DBHeader hdr;
int i;
char flg=0x0D;
time_t ltime;
struct tm * ttm;

memset((void*)&hdr, 0x00, sizeof(DBHeader));

time(&ltime);
ttm = localtime(&ltime);
hdr.last_update[0] = (char)ttm->tm_year - 100;
hdr.last_update[1] = (char)ttm->tm_mon + 1;
hdr.last_update[2] = (char)ttm->tm_mday + 1;


hdr.identity = 0x03;
hdr.no_of_record=0;
hdr.language_driver_id=0x03;

hdr.record_size = 1; // initialize ; 1 for a flag byte
for(i=0;i<nFields;i++) hdr.record_size += FieldList[i].field_length;

hdr.nFields=nFields;
hdr.header_size = (unsigned short)(sizeof(DBHeader) + nFields * sizeof(DBField) + 1);

strcpy(fl_extn, ".tab");

sprintf(fname,"%s%s%s%s%s%s%s%s%s%s",HDB_BASE,DIR_SEP,VERSION,DIR_SEP,"data",DIR_SEP,UserName,DIR_SEP,TableName, fl_extn);
fp=fopen(fname,"wb");
if(fp==NULL) return ERR_BADTABLE;

fwrite(&hdr,sizeof(DBHeader),1,fp);
fwrite(FieldList,sizeof(DBField),nFields,fp);

fwrite(&flg, 1, 1, fp);
fclose(fp);
return true;
}*/

int chop39(char * val)
{
	int i, l = strlen(val);
	if (val[0] == 39 && l >= 2) // "'"
	{
		for (i = 0; i < l; i++)
		{
			val[i] = val[i + 1];
		}
		val[l - 2] = 0;
	}
	return true;
}

int chop_lzero(char * val)
{
	int point = -1, i, l = strlen(val);
	for (i = l - 1; i >= 0; i--)
	{
		if (val[i] == '.') { point = i; break; }
	}
	if (point >= 0)
	{
		for (i = l - 1; i >= point; i--)
		{
			if (val[i] == '0' || val[i] == '.') val[i] = 0x00; else break;
		}
	}
	return true;
}


/*int dOutputResult(DBResult * result)
{

}

int dCloseResult(DBResult * result)
{

}


DBResult * dCreateResult()
{

}

DBRow * dAddRow(DBResult * result)
{

}*/


int dGetFunctionCode(char * func_name)
{
	return false;
}

char dGetAggrCode(char * aggr_name)
{
	if (strcmp("COUNT", aggr_name) == 0) return AGGR_COUNT;
	if (strcmp("SUM", aggr_name) == 0) return AGGR_SUM;
	if (strcmp("MAX", aggr_name) == 0) return AGGR_MAX;
	if (strcmp("MIN", aggr_name) == 0) return AGGR_MIN;
	if (strcmp("AVG", aggr_name) == 0) return AGGR_AVG;
	return false;
}

char dGetAggrType(char aggr_code)
{
	switch (aggr_code)
	{
	case AGGR_COUNT: return DT_NUMBER;
	case AGGR_SUM: return DT_NUMBER;
	case AGGR_AVG: return DT_NUMBER;
	case AGGR_MAX: return DT_UNKNOWN;
	case AGGR_MIN: return DT_UNKNOWN;
	}
	return false;
}

/*
int isRsvWord(char * word)
{
#define MAX_NRSV 33
char RsvWords[MAX_NRSV][11]=
{"CREATE","TABLE", "FROM", "DESC", "IN", "BETWEEN", "LIKE",
"INSERT", "INTO", "ORDER", "WHERE", "GROUP", "BY", "DIV", "MOD",
"SELECT", "DISTINCT", "DATE", "TIME", "DATETIME", "VARCHAR",
"VARCHAR2", "CHAR", "NUMBER", "INT", "INT2", "INT4", "BLOB",
"ASC", "DSC", "CONSTRAINT", "SET", ""};
int i;

HDB_ASSERT(EQUAL(RsvWords[MAX_NRSV-1], ""));
for(i=0; i<MAX_NRSV; i++)
{
if(EQUAL(RsvWords[i], word)) return true;
}
return false;
}
*/
