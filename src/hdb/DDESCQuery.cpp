// DDESCQuery.cpp: implementation of the DDESCQuery class.
//
//////////////////////////////////////////////////////////////////////

#include "DDESCQuery.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DDESCQuery::DDESCQuery(DSession *ssn, char * sql) : DQuery(ssn, sql)
{

}

DDESCQuery::~DDESCQuery()
{

}

int DDESCQuery::Parse()
{
	return TRUE;
}

int DDESCQuery::Execute()
{
	int ei, i, retcd = true;
	DTable * tbl = new DTable(m_ssn, m_stl[1]);
	DRow * crow = NULL;

	tbl->Open();

	if (tbl == NULL)
	{
		retcd = ERR_BADTABLE;
		goto CLEANUP;
	}

	m_result->m_nCol = 5;

	m_result->m_FieldList = (tagDBField*)malloc(m_result->m_nCol * sizeof(tagDBField));
	ei = 0;
	strncpy(m_result->m_FieldList[ei].m_field_name, "FIELD_NAME", FIELD_NAME_WIDTH - 1);
	m_result->m_FieldList[ei].m_field_name[FIELD_NAME_WIDTH - 1] = 0;
	m_result->m_FieldList[ei].m_field_length = FIELD_NAME_WIDTH;
	ei++;
	strncpy(m_result->m_FieldList[ei].m_field_name, "NULL", FIELD_NAME_WIDTH - 1);
	m_result->m_FieldList[ei].m_field_length = 4;
	ei++;
	strncpy(m_result->m_FieldList[ei].m_field_name, "DATA_TYPE", 12);
	m_result->m_FieldList[ei].m_field_length = 12;
	ei++;
	strncpy(m_result->m_FieldList[ei].m_field_name, "LENGTH", 9);
	m_result->m_FieldList[ei].m_field_length = 9;
	ei++;
	strncpy(m_result->m_FieldList[ei].m_field_name, "DECIMAL", 9);
	m_result->m_FieldList[ei].m_field_length = 9;


	for (i = 0; i < tbl->GetFieldCount(); i++)
	{
		char * tt;
		crow = m_result->AddRow();

		tt = (char*)malloc((FIELD_NAME_WIDTH + 1)*sizeof(char));
		strcpy(tt, tbl->GetTagField(i)->m_field_name);
		crow->m_arValues[0] = tt;

		tt = (char*)malloc(FIELD_NAME_WIDTH*sizeof(char));
		if (tbl->GetTagField(i)->m_is_null == 'N') strcpy(tt, "No"); else tt[0] = 0;
		crow->m_arValues[1] = tt;

		crow->m_arValues[2] = dGetDataTypeName(tbl->GetTagField(i)->m_field_type);

		tt = (char*)malloc(FIELD_NAME_WIDTH*sizeof(char));
		sprintf(tt, "%d", tbl->GetTagField(i)->m_field_length);
		crow->m_arValues[3] = tt;

		tt = (char*)malloc(FIELD_NAME_WIDTH*sizeof(char));
		sprintf(tt, "%d", tbl->GetTagField(i)->m_field_decimal);
		crow->m_arValues[4] = tt;
	}
	retcd = true;
	strcpy(m_result->m_errmsg, "Table Described");
CLEANUP:
	if (tbl != NULL)
		tbl->Close();
	m_errcd = m_result->m_errcd = retcd;
	return TRUE;
}
