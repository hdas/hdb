// DTable.h: interface for the DTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DTABLE_H__9171BA00_EF8F_42DB_BAC6_DFAD88A2C8EA__INCLUDED_)
#define AFX_DTABLE_H__9171BA00_EF8F_42DB_BAC6_DFAD88A2C8EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "db.h"

class DTable
{
public:
	int CreateTable(tagDBField * FieldList, unsigned short nFields);
	int CreateIndex(char ** pFieldNameList, unsigned short nFields, char * IndexName);
	int Unlock();
	int Lock();
	int DeleteRecord();
	int AppendRecord();
	int RestoreField(int FieldCode, char * Value);
	char * FetchField(int FieldCode, int * retcd);
	int NextRecord();
	int FirstRecord();
	char FieldType(int FieldCode);
	int FieldSize(int FieldCode);
	int FieldCode(char * FieldName);
	int GetFieldCount();
	tagDBField * GetTagField(char * fieldName);
	tagDBField * GetTagField(int fldCode);

	int Close();
	int Open();
	DTable(DSession * pSsn, char * TableName);
	virtual ~DTable();

private:
	//Public members
	char m_tablename[TABLE_NAME_WIDTH];
	char m_username[TABLE_NAME_WIDTH];
	FILE * m_fp;
	tagDBHeader * m_pHeader;
	DSession * m_pSsn;
	int m_Cached;
	int m_nFields;
	struct tagDBField * m_pTagFields;
	long m_abspos;
	char m_Locked; // bool; determine if locked or unlocked; and lockid must greater than 0 (zero)
	int m_LockId; // depends on locked; has some id (>0) which is also the lockid of Session by which this table is locked 
	char * m_fp_buf;
};

#endif // !defined(AFX_DTABLE_H__9171BA00_EF8F_42DB_BAC6_DFAD88A2C8EA__INCLUDED_)
