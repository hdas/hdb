// DConditionalQuery.cpp: implementation of the DConditionalQuery class.
//
//////////////////////////////////////////////////////////////////////

#include "DConditionalQuery.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DConditionalQuery::DConditionalQuery(DSession *ssn, char * sql) : DQuery(ssn, sql)
{
	//m_nTable = 0;
	//m_paTable = NULL;

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

	for (i = 0; i < m_paTable.size(); i++)
	{
		if (m_paTable[i] != NULL)
		{
			m_paTable[i]->Close();
			delete m_paTable[i];
		}
	}
	m_paTable.clear();
}

//
// This function parse the where clause
//
int DConditionalQuery::ParseCondition(int st_where, int nd_where)
{
	int tmp_lt = 0;
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
			//TODO: re-write for function support; beacause it may contains ","
			if (EQUAL(m_tokens[tn], ",") || EQUAL(m_tokens[tn], ";") || EQUAL(m_tokens[tn], ""))
			{
				break;
			}
			tmp_lt = tn;
		}

		std:pair<short, short> range = { st_where, tmp_lt };
		m_pWhereExpr = new DExpr(m_paTable, m_tokens, range);
	}

	return SUCCESS;
}

int DConditionalQuery::OpenTableList(int startTokenTable)
{
	// Now opening the tables
	int i;
	int errcd = SUCCESS;
	//m_paTable = (DTable**)malloc(sizeof(DTable*) * m_nTable);

	// This is required; useful at destructor
	// when one table has opened, but not another
	//for (i = 0; i < m_nTable; i++) m_paTable[i] = NULL;

	for (i = startTokenTable; i < m_tokens.size(); i++)
	{
		if (EQUAL(m_tokens[i], ",")) {
			continue;
		}
		else if (EQUAL(m_tokens[i], "WHERE") || EQUAL(m_tokens[i], "ORDER") || EQUAL(m_tokens[i], "GROUP") //for select
			|| EQUAL(m_tokens[i], "SET") // For Update
			|| EQUAL(m_tokens[i], "VALUES") || EQUAL(m_tokens[i], "(") // For insert
			|| EQUAL(m_tokens[i], ";")
			) {
			break;
		}

		auto table = new DTable(m_ssn, m_tokens[i]);
		int res = table->Open();
		if (res != SUCCESS)
		{
			errcd = ERR_BADTABLE;
			DError * pErr = new DError(errcd, "Can not open table", ERRTYPE_PARSE, startTokenTable + (i * 2));
			this->m_ssn->SetLastError(pErr);
			break;
		}
		m_paTable.push_back(table);
	}

	return errcd;
}

int DConditionalQuery::ParseTableList()
{
	return TRUE;
}
