// DDeleteQuery.cpp: implementation of the DDeleteQuery class.
//
//////////////////////////////////////////////////////////////////////

#include "DDeleteQuery.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DDELETEQuery::DDELETEQuery(DSession *ssn, char * sql) : DDMLQuery(ssn, sql)
{

}

DDELETEQuery::~DDELETEQuery()
{

}

int DDELETEQuery::Parse()
{
	int retcd, st_where=0, tn;


	if (!EQUAL(m_tokens[1], "FROM"))
	{
		m_errcd = ERR_BADSQL;
		return FALSE;
	}

	

	// Determine the Number of Tables
	int nTable = 0;
	for (tn = 2; tn < m_nToken; tn++)
	{
		if (EQUAL(m_tokens[tn], "WHERE"))
		{
			nTable++;
			st_where = tn + 1;
			break;
		}
		if (EQUAL(m_tokens[tn], ""))
		{
			m_errcd = ERR_BADSQL;
			return FALSE;
		}
		if (EQUAL(m_tokens[tn], ",")) nTable++;
		if (EQUAL(m_tokens[tn], ";")){ nTable++; break; }
	}

	if (this->OpenTableList(2) == SUCCESS)
	{
		retcd = DConditionalQuery::ParseCondition(st_where);
		if(retcd == SUCCESS)
			m_bParsed = TRUE;
	}

	return m_bParsed;
}

int DDELETEQuery::Execute()
{
	int i, res = 0, retcd = true;
	bool match = false;
	//DResult * result=NULL;
	//DDMLQuery * dq=NULL;
	DVariable * wcval = NULL;



	m_result = new DResult();
	m_result->m_SqlType = SQL_DML;

	// Checking
	if (m_nToken < 5) HDB_RETURN(ERR_BADSQL);

	//dq = new DDmlQuery(ssn, qry);
	//retcd = CreateDeleteQuery();
	//if(retcd<0) HDB_RETURN(retcd);


	// Must be locked
	for (i = 0; i < m_paTable.size(); i++)
	{
		res = m_paTable[i]->Lock();
		if (res < 0) HDB_RETURN(res);
	}

	res = m_paTable[0]->FirstRecord(); // to re-write for multiple table
	while (res > 0)
	{
		if (m_nWhereExpr != 0)
		{
			wcval = m_pWhereExpr->Evaluate(m_paTable);
			match = (wcval->m_numval == 1);
		}
		else
			match = true;

		if (match)
		{
			m_paTable[0]->DeleteRecord();
			m_result->m_nRow++;
		}
		res = m_paTable[0]->NextRecord(); // to re-write for multiple table
	}
	retcd = true;
	strcpy(m_result->m_errmsg, "Row Deleted");

CLEANUP:
	for (i = 0; i < m_paTable.size(); i++)
	{
		res = m_paTable[i]->Unlock();
	}
	m_result->m_errcd = retcd;
	return TRUE;
}
