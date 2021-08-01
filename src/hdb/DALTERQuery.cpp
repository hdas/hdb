// DALTERQuery.cpp: implementation of the DALTERQuery class.
//
//////////////////////////////////////////////////////////////////////

#include "DALTERQuery.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DALTERQuery::DALTERQuery(DSession *ssn, char * sql) : DDDLQuery(ssn, sql)
{

}

DALTERQuery::~DALTERQuery()
{

}

int DALTERQuery::Parse()
{
	m_bParsed = TRUE;
	return m_bParsed;
}

int DALTERQuery::Execute()
{
	return true;
}
