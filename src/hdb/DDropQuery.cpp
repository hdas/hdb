// DDropQuery.cpp: implementation of the DDropQuery class.
//
//////////////////////////////////////////////////////////////////////

#include "DDropQuery.h"
#include "DQueryFactory.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DDROPQuery::DDROPQuery(DSession *ssn, char * sql) : DDDLQuery(ssn, sql)
{

}

DDROPQuery::~DDROPQuery()
{

}

int DDROPQuery::Parse()
{
	m_bParsed = TRUE;
	return m_bParsed;
}

int DDROPQuery::Execute()
{
	char dlt_sql[256] = "Delete From Cat Where ONAME='";

	strcat(dlt_sql, m_stl[2]);
	strcat(dlt_sql, "'");

	DQuery * qry = DQueryFactory::CreateQuery(m_ssn, dlt_sql);
	if (qry->Parse())
	{
		qry->Execute();
	}


	if (qry->GetResult()->m_errcd != TRUE)
	{
		delete qry;
		m_errcd = m_result->m_errcd = qry->GetResult()->m_errcd;
		return FALSE;
	}
	else
	{
		if (qry->GetResult()->m_nRow < 1)
		{
			delete qry;
			m_errcd = m_result->m_errcd = qry->GetResult()->m_errcd;
			return FALSE;
		}
	}

	m_errcd = m_result->m_errcd = TRUE;
	strcpy(m_result->m_errmsg, "Table Dropped");
	return TRUE;
}
