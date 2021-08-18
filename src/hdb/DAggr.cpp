// DAggr.cpp: implementation of the DAggr class.
//
//////////////////////////////////////////////////////////////////////

#include "DAggr.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DAggr::DAggr()
{

}

DAggr::~DAggr()
{

}

int DAggr::CreateAggr(char *aggr_name, DExpr *the_expr_in_aggr, int *retcd)
{
	//DBAggr * aggr = (DAggr*)malloc(sizeof(DAggr));

	m_aggr_code = dGetAggrCode(aggr_name);
	//m_datatype = dGetAggrType(m_aggr_code);
	m_pExpr = the_expr_in_aggr;
	//m_nExpr = nExpr;
	m_process_count = 0;

	switch (m_aggr_code)
	{
	case AGGR_COUNT:
	{
		m_numval = 0;
		m_datatype = DT_NUMBER;
	}
	case AGGR_SUM:
	{
		m_numval = 0;
		m_datatype = DT_NUMBER;
	}
	case AGGR_MAX:
	{
		m_numval = 0;
		m_datatype = DT_UNKNOWN;
	}
	case AGGR_MIN:
	{
		m_numval = 0;
		m_datatype = DT_UNKNOWN;
	}
	case AGGR_AVG:
	{
		m_numval = 0;
		m_datatype = DT_NUMBER;
	}

	}

	if (m_datatype == DT_NUMBER) // for count,sum,avg
	{
		m_numval = 0;
		m_strval = NULL;
	}
	else if (m_datatype == DT_UNKNOWN) // for max,min
	{
		m_strval = (char*)malloc(sizeof(char)*MAX_CHAR_LENGTH);
		m_strval[0] = 0x00;
	}
	else
	{
		*retcd = FALSE;
		//free(aggr);
		return NULL;
	}

	*retcd = TRUE;
	return SUCCESS;
}

int DAggr::ProcessAggr(std::vector<DTable *> &paTable)
{
	int intVal = 0;
	double numval = 0;
	DVariable * var = NULL;

	if (m_aggr_code == AGGR_COUNT)
	{
		m_numval += 1;
	}
	else
	{
		var = m_pExpr->Evaluate(paTable);
		HDB_ASSERT(var != NULL);
		if (var->m_VarClass == VarClass::Value && var->m_DataType == DT_NUMBER)
		{
			HDB_ASSERT(var->m_strval != NULL);
			numval = atof(var->m_strval);
			m_datatype = DT_NUMBER;
		}
		else if (var->m_VarClass == VarClass::Value && (var->m_DataType == DT_CHAR || var->m_DataType == DT_VARCHAR))
		{
			HDB_ASSERT(var->m_strval != NULL);
			//var->strval
			m_datatype = DT_CHAR;
		}

	}


	if (m_aggr_code == AGGR_SUM)
	{
		HDB_ASSERT(m_datatype == DT_NUMBER);
		m_numval += numval;
	}
	else if (m_aggr_code == AGGR_MAX)
	{
		if (m_datatype == DT_NUMBER)
		{
			if (m_process_count == 0) // init
			{
				m_numval = numval;
			}
			else
			{
				m_numval = (m_numval > numval) ? m_numval : numval;
			}
		}
		else if (m_datatype = DT_CHAR)
		{
			if (m_process_count == 0) // init
			{
				if (m_strval != NULL) free(m_strval);
				m_strval = (char*)malloc(strlen(var->m_strval) + 1);
				strcpy(m_strval, var->m_strval);
			}
			else
			{
				if (strcmp(m_strval, var->m_strval) < 0)
				{
					if (m_strval != NULL) free(m_strval);
					m_strval = (char*)malloc(strlen(var->m_strval) + 1);
					strcpy(m_strval, var->m_strval);

				}
			}
		}
	}
	else if (m_aggr_code == AGGR_MIN)
	{
		if (m_datatype == DT_NUMBER)
		{
			if (m_process_count == 0) // init
			{
				m_numval = numval;
			}
			else
			{
				m_numval = (m_numval < numval) ? m_numval : numval;
			}
		}
		else if (m_datatype = DT_CHAR)
		{
			if (m_process_count == 0) // init
			{
				if (m_strval != NULL) free(m_strval);
				m_strval = (char*)malloc(strlen(var->m_strval) + 1);
				strcpy(m_strval, var->m_strval);
			}
			else
			{
				if (strcmp(m_strval, var->m_strval) > 0)
				{
					if (m_strval != NULL) free(m_strval);
					m_strval = (char*)malloc(strlen(var->m_strval) + 1);
					strcpy(m_strval, var->m_strval);

				}
			}
		}
	}
	else if (m_aggr_code == AGGR_AVG)
	{
		HDB_ASSERT(m_datatype == DT_NUMBER);
		m_numval += numval; // devide when release
	}

	m_process_count++;

	if (var)
		delete var;

	return true;
}

char * DAggr::ReleaseAggr(int *retcd)
{
	char * value;
	value = (char*)malloc(sizeof(char)*MAX_CHAR_LENGTH);
	value[0] = 0;

	if (m_aggr_code == AGGR_AVG)
	{
		m_numval = m_numval / m_process_count; // devide when release
	}

	if (m_datatype == DT_NUMBER)
	{
		sprintf(value, "%f", m_numval);
	}
	else
	{
		strcpy(value, m_strval);
	}

	chop39(value);
	chop_lzero(value);

	*retcd = true;
	return value;
}

int DAggr::DestroyAggr()
{
	if (m_datatype == DT_UNKNOWN)
	{
		free(m_strval);
	}
	//	free(aggr);
	return true;
}


