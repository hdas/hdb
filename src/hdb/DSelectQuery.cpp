// DSELECTQuery.cpp: implementation of the DSELECTQuery class.
//
//////////////////////////////////////////////////////////////////////

#include "DSELECTQuery.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DSELECTQuery::DSELECTQuery(DSession *ssn, char * sql) : DConditionalQuery(ssn, sql)
{
	m_nGroupByExpr = 0;
	m_paGroupByExpr = NULL;

	m_nOrderByExpr = 0;
	m_paOrderByExpr = NULL;
}

DSELECTQuery::~DSELECTQuery()
{
	int i;

	if (m_paOutExpr.size() > 0)
	{
		for (i = 0; i < m_paOutExpr.size(); i++)
		{
			if (m_paOutExpr[i] != NULL)
			{
				delete m_paOutExpr[i];
				m_paOutExpr[i] = NULL;
			}
		}
		m_paOutExpr.clear();
	}

	if (m_paOrderByExpr != NULL)
	{
		for (i = 0; i < m_nOrderByExpr; i++)
		{
			if (m_paOrderByExpr[i] != NULL)
			{
				delete m_paOrderByExpr[i];
				m_paOrderByExpr[i] = NULL;
			}
		}
		free(m_paOrderByExpr);
		m_paOrderByExpr = NULL;
	}


	if (m_paGroupByExpr != NULL)
	{
		for (i = 0; i < m_nGroupByExpr; i++)
		{
			if (m_paGroupByExpr[i] != NULL)
			{
				delete m_paGroupByExpr[i];
				m_paGroupByExpr[i] = NULL;
			}
		}
		free(m_paGroupByExpr);
		m_paGroupByExpr = NULL;
	}

}

int DSELECTQuery::Parse()
{
	int ei, i, ti, tn, res = false, retcd = false;
	int tmp_st = 0, next_tmp_st = 0, tmp_lt = 0;

	m_bParsed = FALSE;

	DExpr * expr_arr = NULL;

	char bFROMStarted = false, bWHEREStarted = false,
		bORDERBYStarted = false, bGROUPBYStarted = false;
	int is_aggr = false, nCol = 1, BEFORE_FROM_CLAUSE = true;
	
	int st_table = 0, st_outexpr = 0, st_where = 0, st_orderby = 0, st_groupby = 0; // the start token of corresponding entity

	std::pair<short, short> rangeTable, rangeOutExpr, rangeWhere, rangeOrderBy, rangeGroupBy;
	rangeTable = rangeOutExpr = rangeWhere = rangeOrderBy = rangeGroupBy = { 0, 0 };


	// Followings are for calculating number of OutExpr(s), Table(s), GroupBy(s), OrderBy(s) etc
	// First Number of Out Expression(s)
	st_outexpr = 1; // beacause it always after "SELECT"

	for (tn = 1;; tn++)
	{
		if (EQUAL(m_tokens[tn + 1], ",")) // more work to do; such as for function, nested sql etc
		{
			//m_nOutExpr++;
			continue;
		}
		else if (EQUAL(m_tokens[tn + 1], "FROM"))
		{
			//m_nOutExpr++;
			bFROMStarted = true;
			break;
		}
		else if (EQUAL(m_tokens[tn + 1], ""))
			break;
	}

	tn++; // increment to overtake "FROM"
	tn++; // This token is the first Table Name

	// Now the number of tables/nested queries and
	// The number of where expression is always 0 or 1; so loop not required; just check
	st_table = tn;

	rangeTable.first = tn;
	
	unsigned short nTable = 0;
	for (;; tn++)
	{
		if (tn >= m_nToken)
			HDB_RETURN(ERR_BADSQL);

		if (EQUAL(m_tokens[tn + 1], ",")) // more work to do; such as for function, nested sql etc
		{
			nTable++;
			rangeTable.second = tn + 2;
			continue;
		}
		else if (EQUAL(m_tokens[tn + 1], "WHERE"))
		{
			nTable++;
			bWHEREStarted = true;
			st_where = tn + 2;
			rangeWhere.first = tn + 2;
			continue;
		}
		else if (EQUAL(m_tokens[tn + 1], "GROUP") && EQUAL(m_tokens[tn + 2], "BY"))
		{
			if (!bWHEREStarted)
				nTable++;

			bGROUPBYStarted = true;
			st_groupby = tn + 3;
			break;
		}
		else if (EQUAL(m_tokens[tn + 1], "ORDER") && EQUAL(m_tokens[tn + 2], "BY"))
		{
			if (!bWHEREStarted)
				nTable++;

			bORDERBYStarted = true;
			st_orderby = tn + 3;
			break;
		}
		else if (EQUAL(m_tokens[tn + 1], ";")) // more work to do; such as for function, nested sql etc
		{
			if (!bWHEREStarted && !bORDERBYStarted && !bGROUPBYStarted)
				nTable++;

			break;
		}
		else if (EQUAL(m_tokens[tn + 1], ""))
			break;
	}

	// Check if nTable is zero; else OPEN the tables
	if (nTable == 0)
	{
		HDB_RETURN(ERR_BADSQL);
	}
	else if (this->OpenTableList(st_table) != SUCCESS) {
		return FAILURE;
	}

	// Now number of group by expr(s)
	if (bGROUPBYStarted)
	{

	}

	// Now number of group by expr(s)
	if (bORDERBYStarted)
	{

	}

	if (bWHEREStarted)
	{
		retcd = DConditionalQuery::ParseCondition(st_where);
		if (retcd != SUCCESS)
		{
			return retcd;
		}
	}

	//*************************************	
	if (EQUAL(m_tokens[st_outexpr], "*")) // all fields
	{
		ei = 0;

		//for (ti = 0; ti < m_nTable; ti++) m_nOutExpr += m_paTable[ti]->GetFieldCount();


		for (ti = 0; ti < m_paTable.size(); ti++)
		{
			for (i = 0; i < m_paTable[ti]->GetFieldCount(); i++)
			{
				std::vector<char*> tmptokens;
				tmptokens.push_back(m_paTable[ti]->GetTagField(i)->m_field_name);
				std::pair<short, short> range = { 0, 0 };
				auto expr = new DExpr(m_paTable, tmptokens, range);
				m_paOutExpr.push_back(expr);
				ei++;
			}
		}
	}
	else
	{
		/*m_paOutExpr = (DExpr**)malloc(sizeof(DExpr*) * m_nOutExpr);
		for (i = 0; i < m_nOutExpr; i++) m_paOutExpr[i] = NULL;*/

		tmp_st = st_outexpr;
		bool done = false;
		while(!done)
		{
			for (i = tmp_st; i < m_nToken; i++) {
				//TODO: re-write for function support; beacause it may contains ","
				if (EQUAL(m_tokens[i], ",")) {
					tmp_lt = i - 1;
					next_tmp_st = i + 1;
					break;
				} else if (EQUAL(m_tokens[i], "FROM")) {
					tmp_lt = i - 1;
					done = true;
					break;
				}
			}

			std::pair<short, short> range = { tmp_st, tmp_lt };
			auto expr = new DExpr(m_paTable, m_tokens, range);
			m_paOutExpr.push_back(expr);
			tmp_st = next_tmp_st;
		}
	}

	is_aggr = DetermineExprArrayForAggr(expr_arr, nCol);

	// computing expr length (data length) and the caption
	ComputeExprArray_Caption_And_Length(expr_arr, nCol, m_paTable[0]);

	// checking Aggr w.r.t Non Aggr rows in sql
	//if(is_aggr)
	//{
	//	retcd = true;
	//	for(ei=0; ei<nCol; ei++) if(expr_arr[ei].aggr_arr == NULL) retcd = ERR_AGGR_N_AGGR;
	//}

	//if(retcd!=true) HDB_RETURN(retcd);

CLEANUP:

	// freeing must be done by calling function after checking value of retcd
	m_result->m_errcd = m_errcd = retcd;
	if (m_errcd >= 0)
	{
		m_bParsed = TRUE;
	}

	return m_bParsed;
}

int DSELECTQuery::Execute()
{
	DVariable * vtvar = NULL;
	int i, ei = 0, rec_res = false, res = false, retcd = true;
	char match = false, is_aggr = false;
	DRow * crow = NULL;

	if (!m_bParsed)
		return FAILURE;

	// initializing rowlist for storing records
	m_result->m_FirstRow->m_arValues = (char**)malloc(m_paOutExpr.size() * sizeof(char*));
	m_result->m_nCol = m_paOutExpr.size();

	m_result->m_FieldList = (tagDBField*)malloc(m_paOutExpr.size() * sizeof(tagDBField));
	HDB_ASSERT(m_result->m_FieldList != NULL);
	for (ei = 0; ei < m_paOutExpr.size(); ei++)
	{
		strncpy(m_result->m_FieldList[ei].m_field_name, m_paOutExpr[ei]->GetCaption(), FIELD_NAME_WIDTH - 1);
		m_result->m_FieldList[ei].m_field_length = m_paOutExpr[ei]->GetLength();
		m_result->m_FieldList[ei].m_field_type = DT_CHAR;
		m_result->m_FieldList[ei].m_is_null = 0;
	}


	for (i = 0; i < m_paTable.size(); i++)
	{
		rec_res = m_paTable[i]->FirstRecord();
		if (rec_res == ERR_EOF) break;
	}
	for (;;) // seems infinite but ERR_EOF occers at end of record
	{
		if (rec_res == 1)
		{
			if (m_pWhereExpr != NULL)
			{
				vtvar = m_pWhereExpr->Evaluate(m_paTable);
				if (vtvar->m_DataType != DT_BOOLEAN) HDB_RETURN(ERR_BADEXPR);
				match = (vtvar->m_numval == 1) ? true : false;
				delete vtvar;
			}
			else match = true;

			if (match)
			{
				if (is_aggr)
				{
					//for(ei=0; ei<nCol; ei++) dProcessAggr(expr_arr[ei].aggr_arr, tbl, 1);
				}
				else
				{
					crow = m_result->AddRow(); // createing a new row 
					for (ei = 0; ei < m_paOutExpr.size(); ei++)
					{
						vtvar = m_paOutExpr[ei]->Evaluate(m_paTable);
						crow->m_arValues[ei] = vtvar->GetStringValue();
						delete vtvar;
					}
				}
			} // of if(match)
		} // good
		else if (rec_res == ERR_EOF) // Normal 
		{
			if (is_aggr) // Releaseing the Aggregate
			{
				crow = m_result->AddRow();
				for (ei = 0; ei < m_paOutExpr.size(); ei++)
				{
					//HDB_ASSERT(expr_arr[ei].aggr_arr != NULL);
					//crow->arValues[ei] = dReleaseAggr(expr_arr[ei].aggr_arr, &res);
					//if(res!=true) HDB_RETURN(res);
				}
			}
			break;
		}
		else HDB_RETURN(rec_res);

		// Now the record pointer have to move
		for (i = 0; i < m_paTable.size(); i++)
		{
			rec_res = m_paTable[i]->NextRecord();
			if (rec_res == 1) break;
			else if (rec_res == ERR_EOF)
			{
				if (i < m_paTable.size() - 1) // to see that next another table exist
					rec_res = m_paTable[i]->FirstRecord();
				if (rec_res == ERR_EOF) break; // may happen where table contains no records
			}
			else HDB_RETURN(rec_res);
		}
	} // of while(rec_res == true)

	retcd = true;
	strcpy(m_result->m_errmsg, "Row Selected");

CLEANUP:

	//dDestroySelectQuery(pSQry);

	m_result->m_errcd = retcd;
	//GetErrMsg(result->retcd, result->retmsg);
	return TRUE;
}

int DSELECTQuery::DetermineExprArrayForAggr(DExpr *expr_arr, int nExpr)
{
	return true;
}

int DSELECTQuery::ComputeExprArray_Caption_And_Length(DExpr *expr_arr, int nCol, DTable *tbl)
{
	return false;
}
