// DDMLQuery.cpp: implementation of the DDMLQuery class.
//
//////////////////////////////////////////////////////////////////////


#include "DDMLQuery.h"
#include "DTable.h"
#include "DVariable.h"
#include "DExpr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DDMLQuery::DDMLQuery(DSession *ssn, char * sql) : DConditionalQuery(ssn, sql)
{
	m_nInExpr = 0;
	m_paInExpr = NULL;



	m_nVarToSet = 0;
	m_paVarToSet = NULL;
}

DDMLQuery::~DDMLQuery()
{
	int i;

	if (m_paInExpr != NULL)
	{
		for (i = 0; i < m_nInExpr; i++)
		{
			if (m_paInExpr[i] != NULL)
			{
				delete m_paInExpr[i];
				m_paInExpr[i] = NULL;
			}
		}
		free(m_paInExpr);
		m_paInExpr = NULL;
	}



	if (m_paVarToSet != NULL)
	{
		for (i = 0; i < m_nVarToSet; i++)
		{
			if (m_paVarToSet[i] != NULL)
			{
				delete m_paVarToSet[i];
				m_paVarToSet[i] = NULL;
			}
		}
		free(m_paVarToSet);
		m_paVarToSet = NULL;
	}
}
