// DConditionalQuery.cpp: implementation of the DConditionalQuery class.
//
//////////////////////////////////////////////////////////////////////

#include "DConditionalQuery.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DConditionalQuery::DConditionalQuery(DSession *ssn, char * sql) : DQuery(ssn, sql)
{
	m_nTable = 0;
	m_paTable = NULL;

	m_nWhereExpr = 0;
	m_pWhereExpr = NULL;
}

DConditionalQuery::~DConditionalQuery()
{
	int i;
	if (m_pWhereExpr != NULL)
	{
		delete m_pWhereExpr;
		m_pWhereExpr = NULL;
	}

	if (m_paTable != NULL)
	{
		for (i = 0; i < m_nTable; i++)
		{
			if (m_paTable[i] != NULL)
			{
				m_paTable[i]->Close();
				delete m_paTable[i];
			}
		}
		free(m_paTable);
	}
}

//
// This function parse the where clause
//
int DConditionalQuery::ParseCondition(int st_where, int nd_where)
{
	int tmp_lt;
	int res = FAILURE;

	if (st_where == 0) {
		while (!EQUAL(m_tokens[st_where], "WHERE")) st_where++;
		st_where++;
	}

	if (st_where > 0)
	{
		m_nWhereExpr = 1;
		for (int tn = st_where; tn < m_tokens.size(); tn++)
		{
			// to re-write for function support; beacause it may contains ","
			if (EQUAL(m_tokens[tn], ",") || EQUAL(m_tokens[tn], ";") || EQUAL(m_tokens[tn], ""))
			{
				tmp_lt = tn - 1;
				break;
			}
		}

		m_pWhereExpr = new DExpr(m_paTable, m_nTable);
		res = m_pWhereExpr->Create(m_tokens, st_where, tmp_lt);
		if (res != SUCCESS)
		{
			m_errcd = res;
			return m_errcd;
		}
	}

	return SUCCESS;
}

int DConditionalQuery::OpenTableList(int startTokenTable)
{
	// Now opening the tables
	int i;
	int errcd = SUCCESS;
	m_paTable = (DTable**)malloc(sizeof(DTable*) * m_nTable);

	// This is required; useful at destructor
	// when one table has opened, but not another
	for (i = 0; i < m_nTable; i++) m_paTable[i] = NULL;

	for (i = 0; i < m_nTable; i++)
	{
		m_paTable[i] = new DTable(m_ssn, m_stl[startTokenTable + (i * 2)]);

		int res = m_paTable[i]->Open();

		if (res != SUCCESS)
		{
			errcd = ERR_BADTABLE;
			DError * pErr = new DError(errcd, "Can not open table", ERRTYPE_PARSE, startTokenTable + (i * 2));
			this->m_ssn->SetLastError(pErr);
			break;
		}
	}

	return errcd;
}

int DConditionalQuery::ParseTableList()
{
	return TRUE;
}
