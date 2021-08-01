// DInsertQuery.cpp: implementation of the DInsertQuery class.
//
//////////////////////////////////////////////////////////////////////

#include "DInsertQuery.h"
#include "DExpr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DINSERTQuery::DINSERTQuery(DSession *ssn, char * sql) : DDMLQuery(ssn, sql)
{

}

DINSERTQuery::~DINSERTQuery()
{

}

int DINSERTQuery::Parse()
{
	int st_vartoset = -1, st_val = -1, next_var, i, tn, tmp_lt;

	if (!EQUAL(m_stl[1], "INTO")) { m_errcd = ERR_BADSQL; return m_errcd; }

	// Determine the Number of Tables
	st_vartoset = 0;
	m_nTable = 0;
	for (tn = 2; tn < m_nToken; tn++)
	{
		if (EQUAL(m_stl[tn], "(")){ m_nTable++; st_vartoset = tn + 1; break; }
		if (EQUAL(m_stl[tn], "")) { m_errcd = ERR_BADSQL; return m_errcd; }
		if (EQUAL(m_stl[tn], ",")) m_nTable++;
		if (EQUAL(m_stl[tn], "VALUES")){ m_nTable++; st_val = tn + 2; break; }
	}

	if (this->OpenTableList(2) != SUCCESS)
	{
		return m_ssn->GetLastError()->GetErrorCode();
	}

	// Determine the Number of VarToSet
	if (st_vartoset > 0)
	{
		m_nVarToSet = 0;
		for (tn = st_vartoset; tn < m_nToken; tn++)
		{
			if (EQUAL(m_stl[tn], ")")){ m_nVarToSet++; }
			if (EQUAL(m_stl[tn], ",")) m_nVarToSet++; // To re-write for function support
			if (EQUAL(m_stl[tn], "VALUES")) { st_val = tn + 2; break; }
			if (EQUAL(m_stl[tn], "") || EQUAL(m_stl[tn], ";")) { m_errcd = ERR_BADSQL; return m_errcd; }
		}
	}
	else
	{
		m_nVarToSet = m_paTable[0]->GetFieldCount();
	}

	m_nInExpr = m_nVarToSet;
	m_paInExpr = (DExpr**)malloc(sizeof(DExpr*) * m_nInExpr);
	m_paVarToSet = (DVariable**)malloc(sizeof(DVariable*) * m_nVarToSet);
	for (i = 0; i < m_nInExpr; i++)
	{
		m_paInExpr[i] = NULL;
		m_paVarToSet[i] = NULL;
	}

	// Creating VarToSet
	if (st_vartoset > 0)
	{
		for (i = 0; i < m_nVarToSet; i++)
		{
			m_paVarToSet[i] = DVariable::CreateFieldVariable(m_stl[st_vartoset + i * 2], m_paTable, m_nTable, &m_errcd);
			if (m_errcd != SUCCESS) return m_errcd;
		}
	}
	else
	{
		for (i = 0; i < m_nVarToSet; i++)
		{
			m_paVarToSet[i] = DVariable::CreateFieldVariable(m_paTable[0]->GetTagField(i)->m_field_name, m_paTable, m_nTable, &m_errcd);
			if (m_errcd != SUCCESS)
				return m_errcd;
		}
	}

	// Creating Values
	if (st_val > 0)
	{
		next_var = st_val;
		tn = next_var;
		for (i = 0; i < m_nInExpr; i++)
		{
			// Finding the Last Token of InExpr
			for (tn = next_var; tn < m_tokens.size(); tn++)
			{
				if (EQUAL(m_tokens[tn], ",") || EQUAL(m_tokens[tn], ";") || EQUAL(m_tokens[tn], ")"))
				{
					tmp_lt = tn - 1;
					break;
				}
			}

			m_paInExpr[i] = new DExpr(m_paTable, m_nTable);
			m_errcd = m_paInExpr[i]->Create(m_tokens, next_var, tmp_lt);
			if (m_errcd != SUCCESS)
			{
				return m_errcd;
			}
			next_var = tn + 1;
		}
	}

	m_bParsed = TRUE;
	return TRUE;
}

int DINSERTQuery::Execute()
{
	int retcd, res, i, ei;
	char ** Values = NULL;
	DVariable * vtvar = NULL;

	if (!m_bParsed)
		return FALSE;

	// Must be locked
	res = m_paTable[0]->Lock();
	if (res != true) HDB_RETURN(res);

	// Now Appending a blank record
	res = m_paTable[0]->AppendRecord();
	if (res != true) HDB_RETURN(res);

	Values = (char**)malloc(sizeof(char*) * m_nInExpr);
	for (i = 0; i < m_nInExpr; i++) Values[i] = NULL;

	// Now restoring the values into the blank record
	for (ei = 0; ei < m_nVarToSet; ei++)
	{
		vtvar = m_paInExpr[ei]->Evaluate(&retcd); // DVariable::eval_with_table(m_paInExpr[ei], m_paTable, m_nTable, &retcd);
		if (retcd < 0)
		{
			HDB_RETURN(retcd);
		}

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
		if (res != true)
		{
			m_paTable[0]->DeleteRecord();
			HDB_RETURN(res);
		}
	}

	retcd = true;
	retcd = CheckConstraints(m_paTable[0], 1);
	if (retcd != true)
	{
		m_paTable[0]->DeleteRecord();
	}
	else
	{
		m_result->m_nRow += 1;
		strcpy(m_result->m_errmsg, "Row Inserted");
	}

CLEANUP:
	if (m_paTable[0] != NULL)
		m_paTable[0]->Unlock();


	if (Values != NULL)
	{
		for (ei = 0; ei < m_nInExpr; ei++)
			if (Values[ei] != NULL) free(Values[ei]);
		free(Values);
	}

	m_result->m_errcd = retcd;
	return TRUE;
}

int DINSERTQuery::CheckConstraints(DTable *tbl, int nTbl)
{
	int retcd = true, i;
	char * value;

	for (i = 0; i < tbl->GetFieldCount(); i++)
	{
		if (tbl->GetTagField(i)->m_is_null == 'N')
		{
			value = tbl->FetchField(i, &retcd);
			if (retcd != true) goto CLEANUP;
			rtrim(value);
			if (EQUAL(value, "")) // Null checking
			{
				free(value);
				HDB_RETURN(ERR_NOT_NULL_FAILED);
			}
			free(value);
		}
	}
CLEANUP:
	return retcd;
}
