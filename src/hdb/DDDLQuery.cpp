// DDDLQuery.cpp: implementation of the DDDLQuery class.
//
//////////////////////////////////////////////////////////////////////

#include "DDDLQuery.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DDDLQuery::DDDLQuery(DSession *ssn, char * sql) : DQuery(ssn, sql)
{
	m_result->m_SqlType = SQL_DDL;
}

DDDLQuery::~DDDLQuery()
{

}

/*int DDDLQuery::Parse()
{
m_bParsed = TRUE;
return m_bParsed;
}

int DDDLQuery::Execute()
{
m_bExecuted = TRUE;
return m_bExecuted;
}*/

