// DUpdateQuery.cpp: implementation of the DUpdateQuery class.
//
//////////////////////////////////////////////////////////////////////

#include "DUpdateQuery.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DUPDATEQuery::DUPDATEQuery(DSession *ssn, char * sql) : DDMLQuery(ssn, sql)
{

}

DUPDATEQuery::~DUPDATEQuery()
{

}

int DUPDATEQuery::Parse()
{
	int st_var, st_where, next_var, i, tn, tmp_lt;
	int retcd = FAILURE;
	// Determine the Number of Tables
	st_var = 0;
	int nTable = 0;
	for (tn = 1; tn < m_nToken; tn++)
	{
		if (EQUAL(m_tokens[tn], "SET")){ nTable++; st_var = tn + 1; break; }
		if (EQUAL(m_tokens[tn], "")) { m_errcd = ERR_BADSQL; return m_errcd; }
		if (EQUAL(m_tokens[tn], ",")) nTable++;
	}

	if (this->OpenTableList(1) != SUCCESS)
	{
		return FALSE;
	}

	// Determine the Number of VarToSet
	st_where = 0;
	m_nVarToSet = 0;
	for (tn = st_var; tn < m_nToken; tn++)
	{
		if (EQUAL(m_tokens[tn], "WHERE"))
		{
			m_nVarToSet++;
			DConditionalQuery::ParseCondition(tn+1);
			break;
		}
		if (EQUAL(m_tokens[tn], ",")) m_nVarToSet++; // To re-write for function support
		if (EQUAL(m_tokens[tn], ";")){ m_nVarToSet++; break; }
		if (EQUAL(m_tokens[tn], "")) { m_errcd = ERR_BADSQL; return m_errcd; }
	}

	m_nInExpr = m_nVarToSet;
	m_paInExpr = (DExpr**)malloc(sizeof(DExpr*) * m_nInExpr);
	m_paVarToSet = (DVariable**)malloc(sizeof(DVariable*) * m_nVarToSet);
	for (i = 0; i < m_nInExpr; i++)
	{
		m_paInExpr[i] = NULL;
		m_paVarToSet[i] = NULL;
	}

	// Creating VarToSet & InExpr
	next_var = st_var;
	tn = next_var;
	for (i = 0; i < m_nVarToSet; i++)
	{
		m_paVarToSet[i] = DVariable::CreateFieldVariable(m_tokens[next_var], m_paTable);

		// Finding the Last Token of InExpr
		for (tn = next_var + 2; tn < m_nToken; tn++)
		{
			if (EQUAL(m_tokens[tn], ",") || EQUAL(m_tokens[tn], ";") || EQUAL(m_tokens[tn], "WHERE"))
			{
				tmp_lt = tn - 1;
				break;
			}
		}

		std::pair<short, short> range = { next_var + 2, tmp_lt };
		m_paInExpr[i] = new DExpr(m_paTable, m_tokens, range);
		next_var = tn + 1;
	}

	m_bParsed = TRUE;

	return m_bParsed;
}

int DUPDATEQuery::Execute()
{
	int  ei, retcd = false, i, res = 0;
	char match;
	char ** Values = NULL;
	DVariable * wcval = NULL, *vtvar = NULL;

	if (!m_bParsed)
	{
		return FAILURE;
	}

	Values = (char**)malloc(sizeof(char*) * m_nInExpr);
	for (i = 0; i < m_nInExpr; i++) Values[i] = NULL;

	// Must be locked
	for (i = 0; i < m_paTable.size(); i++)
	{
		res = m_paTable[i]->Lock();
		if (res < 0)
			HDB_RETURN(res);
	}

	res = m_paTable[0]->FirstRecord(); // to re-write for multiple table
	while (res > 0)
	{
		if (m_nWhereExpr != 0)
		{
			wcval = m_pWhereExpr->Evaluate(m_paTable);
			if (wcval->m_numval == 1) match = 1; else match = 0;
		}
		else match = 1;

		if (match)
		{
			for (ei = 0; ei < m_nVarToSet; ei++)
			{
				vtvar = m_paInExpr[ei]->Evaluate(m_paTable);

				Values[ei] = vtvar->GetStringValue();
				delete vtvar;

				// Not Null Checking
				if (m_paTable[m_paVarToSet[ei]->m_ref2]->GetTagField(m_paVarToSet[ei]->m_ref1)->m_is_null == 'N' && EQUAL(Values[ei], ""))
				{
					free(Values[ei]);
					Values[ei] = NULL;
					HDB_RETURN(ERR_NOT_NULL_FAILED);
				}
			}

			for (ei = 0; ei < m_nVarToSet; ei++)
			{
				res = m_paTable[m_paVarToSet[ei]->m_ref2]->RestoreField(m_paVarToSet[ei]->m_ref1, Values[ei]);
				free(Values[ei]);
				Values[ei] = NULL;
				if (res != true)	HDB_RETURN(res);
			}
			m_result->m_nRow++;
		}
		res = m_paTable[0]->NextRecord(); // to re-write for multiple table
	}

	retcd = true;
	strcpy(m_result->m_errmsg, "Row Updated");

CLEANUP:

	for (i = 0; i < m_paTable.size(); i++)
	{
		res = m_paTable[i]->Unlock();
	}

	if (Values != NULL)
	{
		for (ei = 0; ei < m_nInExpr; ei++) if (Values[ei] != NULL) free(Values[ei]);
		free(Values);
	}

	m_errcd = m_result->m_errcd = retcd;
	return TRUE;
}
