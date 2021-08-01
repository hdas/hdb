#ifndef __db_h
#define __db_h

/********************************************************/
// The Oparating System Selection
// it must be match with host Oparating System

//#define OS_LINUX
//#define OS_UNIX
#ifdef WIN32
#define OS_WINDOWS
#else
#define OS_LINUX
#endif

#define VERSION_EXTN "Alfa "
#define INITIAL_VERSION_DATE "01092001"

//#define VERSION "2.3.0"
//#define VERSION_DATE "15122002"

//#define VERSION "2.3.3"
//#define VERSION_DATE "28092003"

//#define VERSION "2.3.4"
//#define VERSION_DATE "02112003"

//#define VERSION "2.3.5"
//#define VERSION_DATE "26112003"

//#define VERSION "2.3.6"
//#define VERSION_DATE "26112003"

//#define VERSION "3.0.0"
//#define VERSION_DATE "21/12/2003"

//#define VERSION "3.0.2"
//#define VERSION_DATE "29/12/2003"

//#define VERSION "3.0.4"
//#define VERSION_DATE "01/01/2004"

//#define VERSION "3.0.5"
//#define VERSION_DATE "05/01/2004"

//#define VERSION "3.1"
//#define VERSION_DATE "18/04/2004"

//#define VERSION "3.1.1"
//#define VERSION_DATE "02/05/2004"

//#define VERSION "4.0"
//#define VERSION_DATE "23/04/2006"

#define VERSION "4.1"
#define VERSION_DATE "01/08/2021"




// define the following to use xBase format table
//#define OPTION_XBASE
// But it is not supported now
/*******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <math.h>
#include <conio.h>

#include <windows.h>
#include <process.h>
#include <tchar.h>

#include <signal.h>

#include <vector>
using namespace std;


#ifdef OS_WINDOWS
#define HDB_BASE "C:\\hdb"
#define DIR_SEP "\\"
#define HOST_OS "Windows 64 bit"
#endif

#ifdef OS_LINUX
#define HDB_BASE "/home/hdb"
#define DIR_SEP "/"
#define HOST_OS "Linux 32 bit"
#endif

#ifdef OS_UNIX
#define HDB_BASE "/home/hdb"
#define DIR_SEP "/"
#define HOST_OS "Unix 32 bit"
#endif


#define DEFAULT_STRLEN 256
#define DEFAULT_NUMLEN 18
#define DEFAULT_USERNAME "hansaraj"
#define FILE_NAME_WIDTH  256

#define MAX_CACHE_TABLE 16

#define FIELD_NAME_WIDTH 32
#define TABLE_NAME_WIDTH 32
#define MAX_CHAR_LENGTH  1024


typedef int BOOL;

//#ifndef OS_WINDOWS
#define TRUE       1
#define FALSE      0

#ifndef OS_WINDOWS
#define true       1
#define false      0
#endif

#define SUCCESS    1
#define FAILURE   -1

// defining query types
#define Q_SELECT    10
#define Q_INSERT    11
#define Q_UPDATE    12
#define Q_DELETE    13
#define Q_ALTER     14
#define Q_CREATE    15
#define Q_COMMIT    16
#define Q_ROLLBACK  17
#define Q_LOCK      18
#define Q_DROP      19
#define Q_DESC      20


// defining standard object types
#define O_TABLE       20
#define O_VIEW        21
#define O_INDEX       22
#define O_EXPRESSION  23
#define O_FUNCTION    24
#define O_AGGREGATE   25
#define O_ROW         26
#define O_RESULT      27
#define O_TEMPTABLE   28
#define O_QUERY       29

// defining oparators
// it must be less then 128 (256 for unsigned char) for storing it in char
#define OP_EQ         '='
#define OP_NE         '#'
#define OP_LT         '<'
#define OP_GT         '>'
#define OP_LE         '{'
#define OP_GE         '}'
#define OP_LIKE       '~'
#define OP_IN         ':'
#define OP_BETWEEN    'B'
#define OP_AND        '&'
#define OP_OR         '|'
#define OP_NOT        '!'
#define OP_MOD        '%'
#define OP_DIV        'D'
#define OP_DIVI       '/'
#define OP_PLUS       '+'
#define OP_UPLUS      'U'
#define OP_MINUS      '-'
#define OP_UMINUS     'N'
#define OP_MULT       '*'
#define OP_EXPN       '^'
#define OP_BRACSTART  '('
#define OP_BRACSTOP   ')'
#define OP_CAT        'C'
#define OP_POW        'P'

// now the reserved words ; Alpha order sorting convention
#define RW_ALTER       10
#define RW_AS          11
#define RW_ASC         12

#define RW_BY          20 

#define RW_CONSTRAINT  30
#define RW_COMMIT      31
#define RW_CREATE      32

#define RW_DATABASE    40
#define RW_DELETE      41
#define RW_DESC        42
#define RW_DISTINCT    43

#define RW_FOREIGN     50
#define RW_FROM        51
#define RW_FUNCTION    52

#define RW_GRANT       53
#define RW_GROUP       54

#define RW_INDEX       55
#define RW_INSERT      57
#define RW_INTO        58

#define RW_KEY         59
#define RW_LIMIT       60
#define RW_OFFSET      61

#define RW_NULL        65

#define RW_ORDER       66

#define RW_PRIMARY     67

#define RW_REVOKE      68
#define RW_ROLLBACK    70

#define RW_WHERE       71

#define RW_SELECT      72
#define RW_SET         73

#define RW_TABLE       74
#define RW_TRUE        75
#define RW_FALSE       76

#define RW_UPDATE      80
#define RW_USER        81

#define RW_VIEW        82
//************ End of ReserveWord




// defining aggr function's ids
#define AGGR_COUNT    'C'
#define AGGR_SUM      'S'
#define AGGR_AVG      'A'
#define AGGR_MAX      'X'
#define AGGR_MIN      'N'

// defining standard DBVariable data types
//#define DT_STRING     10
//#define DT_INT2       11
//#define DT_INT4       12
//#define DT_INT8       13
//#define DT_FLOAT      14
//#define DT_DOUBLE     15
#define DT_NUMBER      'N'
//#define DT_DATE       17
//#define DT_TIME       19
#define DT_DATETIME   'D'
#define DT_VARCHAR    'V'
//#define DT_VARCHAR2   22
#define DT_CHAR       'C'
#define DT_BOOLEAN    'B'
#define DT_BLOB       'L'

//#define DT_VAR        25
//#define DT_FIELDNAME  26
//#define DT_FIELDCODE  27
//#define DT_TABLENAME  28
//#define DT_TABLECODE  29
//#define DT_OPARATOR   30
#define DT_UNKNOWN    'U'

// Defining DBVariable Class
#define VCLS_OPARATOR    'O'
#define VCLS_VALUE       'V'
#define VCLS_FIELD       'F'
#define VCLS_FUNCTION    'N'
#define VCLS_AGGR        'A'
#define VCLS_PARAM       'P'
#define VCLS_DATATYPE    'D'
#define VCLS_RESERVEWORD 'R' 


// Record Status ; stored as flag of each record
#define FLAG_DELETED_RECORD '*'
#define FLAG_LOCKED_RECORD  '#'
#define FLAG_VALID_RECORD   '='
#define FLAG_EOF            0x1A

#define SQL_DDL     'D'
#define SQL_DML     'M'
#define SQL_DCL     'C'
#define SQL_DQL     'Q'


//Error Type
#define ERRTYPE_PARSE    50
#define ERRTYPE_EXECUTE  51
#define ERRTYPE_SECURITY 52

// defining error codes
// some of it must be less than 128 such as ERR_BADFIELD
#define ERR_BADFILE         -190
#define ERR_BADFIELD        -200 // must be less than 256; beacause it may return through a char; e.g. function dFieldType
#define ERR_BADTABLE        -210
#define ERR_BADINDEX        -211
#define ERR_BADNUMBER       -220
#define ERR_BADOPARATOR     -230
#define ERR_BADVALUE        -240
#define ERR_NOREAD          -250
#define ERR_NOWRITE         -260
#define ERR_POSNOTSET       -270
#define ERR_EOF             -280
#define ERR_BADEXPR         -290
#define ERR_BADSQL          -300
#define ERR_NOMEM           -310
#define ERR_BADDATATYPE     -320
#define ERR_TYPEMISMATCH    -330
#define ERR_NOTIMPL         -340
#define ERR_BADTYPE         -350
#define ERR_ALREADYEXIST    -360
#define ERR_BADFIELDNAME    -370
#define ERR_NOT_NULL_FAILED -380
#define ERR_RW_USED         -390 // Reserved Word used
#define ERR_DIVISION_BY_ZERO -400
#define ERR_AGGR_N_AGGR      -410
#define ERR_LARGE_VALUE      -420
#define ERR_LARGE_NAME       -421
#define ERR_DATATYPEMISMATCH -430


/* Macro defination */
#define EQUAL(x,y) (strcmp(x,y)==0)

#define HDB_RETURN(x) { retcd = x; goto CLEANUP; }
#ifdef _DEBUG
#define HDB_ASSERT(x) if(!(x)){printf("HDB_ASSERT failed : file=%s, line=%d.\n", __FILE__, __LINE__ ); exit(2);}
#else
#define HDB_ASSERT(x) {;}
#endif


typedef struct _DBGlobal
{
	char bLogSql; // boolean ; set true to log sql into filename
	char LogSqlPath[MAX_PATH];
	char DatabasePath[MAX_PATH];
	char bListen; // boolean ; listen on socket or not
	int  ListenPort;
	int  nThread;
	int  nConnected;
	char bStopRequest;
} DBGlobal;


struct tagDBHeader
{
	char  m_identity;
	char  m_last_update[3];
	unsigned long m_no_of_record;
	unsigned short m_header_size;
	unsigned short m_record_size;
	unsigned short m_nFields;
	char  m_reserved1[2];
	char  m_incomplete_transaction_flag;
	char  m_encryption_flag;
	char  m_multiuser_reserved[12];
	char  m_mdx_flag;
	char  m_language_driver_id;
	char  m_reserved2[2];
	char  m_reserved3[30];
};
//typedef class DBheader DBHeader;

struct tagDBField
{
	char m_field_name[FIELD_NAME_WIDTH];
	char m_field_type; // Type of the field
	char m_reserved1[4];
	unsigned short m_field_length;
	unsigned char m_field_decimal;
	char m_reserved2[2];
	char m_work_area_id;
	char m_reserved3[10];
	char m_mdx_flag;
	char m_is_null;
	unsigned short m_pos_offset; // contains the position offset
};


struct tagDBIdxHeader
{
	char  m_identity;
	char m_idxname[TABLE_NAME_WIDTH];
	char m_tname[TABLE_NAME_WIDTH];
	char m_isUnique;
	char m_last_update[3];

	unsigned long m_no_of_record;
	unsigned short m_header_size;
	unsigned short m_record_size;
	unsigned short m_nFields;

	char  m_reserved1[2];
	char  m_incomplete_transaction_flag;
	char  m_encryption_flag;
	char  m_multiuser_reserved[12];
	char  m_mdx_flag;
	char  m_language_driver_id;
	char  m_reserved2[2];
	char  m_reserved3[30];
};
//typedef class DBidxheader DBIdxHeader;

extern DBGlobal global_opt;

// Required for Expression
int icp(int oprtr);
int isp(int oprtr);

int rpad(char * str, int len);
int rtrim(char * str);

// d functions : downmost level functions

//DBSession * dCreateSession(char * username);
//int dCloseSession(DBSession * ssn);

// DBVariable
//DBVariable * dCreateVariable(int DataType, int szIfStr, int * retcd);
//DBVariable * dCreateVariable(int * retcd);
//int dStoreVariable(DBVariable * pVar, int DataType, char * strval);
//DBVariable * dCreateDuplicateVariable(DBVariable * pSrcVar, int * retcd);
//int dChangeVariableDatatype(DBVariable * pVar, int NewDataType);
//int dDestroyVariable(DBVariable * pVar);


// DBStack
//int dPushStack(DBStack * top, DBVariable * pVar);
//DBVariable * dPopStack(DBStack * top, int * retcd);

//DBStack * dCreateStack(int * retcd); // implemented
//int dDestroyStack(DBStack * stack); // implemented


/*DBTable * dOpenTable(char * TableName, DBSession * pSsn); // implemented
int dCloseTable(DBTable * tbl); // implemented

int dDropTable(DBTable * aTable, char * UserName);
int dCreateTable(char * TableName,DBField * FieldList,unsigned short nFields,char * UserName);

int dCreateIndex(char * IndexName, char * TableName, DBField * FieldList,unsigned short nFields, char * UserName);

int  dFieldCode(DBTable * tbl, char * FieldName); // implemented
int  dFieldName(DBTable * tbl, int FieldCode);
int  dFieldSize(DBTable * tbl, int FieldCode); // implemented
char dFieldType(DBTable * tbl, int FieldCode); // implemented

char * dFetchField(DBTable * tbl, int FieldCode, int * retcd); // implemented
int dRestoreField(DBTable * tbl,int FieldCode,char * Value); // implemented

int dFetchFieldByName(DBTable * tbl,char * FieldName,char * Value);
int dFetchManyFieldByName(DBTable * tbl,char ** FieldNameList,char ** ValueList);
int dFetchManyFieldByCode(DBTable * tbl,char ** FieldCodeList,char ** ValueList);

int dRestoreFieldByName(DBTable * tbl,char * FieldName,char * Value);
int dRestoreManyFieldByName(DBTable * tbl,char ** FieldNameList,char ** ValueList);
int dRestoreManyFieldByCode(DBTable * tbl,char ** FieldCodeList,char ** ValueList);

int dNextRecord(DBTable * tbl);  // implepented
int dPrevRecord(DBTable * tbl);
int dFirstRecord(DBTable * tbl); // implemented
int dLastRecord(DBTable * tbl);

int dEOF(DBTable * tbl);

int dLockTable(DBSession * ssn, DBTable * tbl);  // Not successfully implemented
int dUnlockTable(DBSession * ssn, DBTable * tbl); // Not successfully implemented

int dAppendRecord(DBTable * tbl); // implemented
int dDeleteRecord(DBTable * tbl); // implemented
*/

char dGetOparatorCode(char * op_str); // implemented
char dGetDataTypeCode(char * dt_str); // implemented
char dGetReserveWordCode(char * rw_str); // implemented
char * dGetDataTypeName(char dt_code); // implemented


//char * dCreateStrVal(DBVariable * var);
int isOparator(char * ctkn);
int isValue(char * ctkn);
int chop39(char * val);
int chop_lzero(char * val);

/*DBResult * dCreateResult();
DBRow * dAddRow(DBResult * result);
int dOutputResult(DBResult * result);
int dCloseResult(DBResult * result);*/

char dGetAggrCode(char * aggr_name);
char dGetAggrType(char aggr_code);

// function
int dGetFunctionCode(char * func_name);

//DBVariable * dCreateFieldVariable(char * FQ_FieldName, DBTable ** paTables, int nTables, int * pRetcd);

//Classed
class DAggr;
class DDMLQuery;
class DResult;
class DRow;
class DSelectQuery;
class DSession;
class DStack;
class DQuery;
class DVariable;
class DTable;
class DExpr;
class DError;


#endif