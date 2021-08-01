#ifndef __aggr_h
#include "aggr.h"
#endif


/*

DBAggr * dCreateAggr(char * aggr_name, DBExpr * the_expr_in_aggr, int * retcd)
{
	DBAggr * aggr = (DBAggr*)malloc(sizeof(DBAggr));

	aggr->aggr_code  = dGetAggrCode(aggr_name);
	//aggr->datatype = dGetAggrType(aggr->aggr_code);
	aggr->pExpr = the_expr_in_aggr;
	//aggr->nExpr = nExpr;
	aggr->process_count=0;

	switch(aggr->aggr_code)
	{
		case AGGR_COUNT:
		{
			aggr->numval = 0;
			aggr->datatype = DT_NUMBER;
		}
		case AGGR_SUM:
		{
			aggr->numval = 0;
			aggr->datatype = DT_NUMBER;
		}
		case AGGR_MAX:
		{
			aggr->numval = 0;
			aggr->datatype = DT_UNKNOWN;
		}
		case AGGR_MIN:
		{
			aggr->numval = 0;
			aggr->datatype = DT_UNKNOWN;
		}
		case AGGR_AVG:
		{
			aggr->numval = 0;
			aggr->datatype = DT_NUMBER;
		}

	}

	if(aggr->datatype==DT_NUMBER) // for count,sum,avg
	{
		aggr->numval=0;
		aggr->strval = NULL;
	}
	else if(aggr->datatype==DT_UNKNOWN) // for max,min
	{
		aggr->strval = (char*)malloc(sizeof(char)*MAX_CHAR_LENGTH);
		aggr->strval[0]=0x00;
	}
	else 
	{
		*retcd=FALSE;
		free(aggr);
		return NULL;
	}

	*retcd = TRUE;
	return aggr;
}

int dProcessAggr(DBAggr * aggr, DBTable ** paTable, int nTable)
{
	int lretcd, intVal=0;
	double numval=0;
	DBVariable * var=NULL;


	if(aggr->aggr_code==AGGR_COUNT)
	{
		aggr->numval += 1;
	}
	else
	{
		var = eval_with_table(aggr->pExpr, paTable, nTable, &lretcd);
		HDB_ASSERT(var!=NULL);
		if(var->VarClass==VCLS_VALUE && var->DataType==DT_NUMBER)
		{
			HDB_ASSERT(var->strval != NULL);
			numval = atof(var->strval);
			aggr->datatype = DT_NUMBER;
		}
		else if(var->VarClass==VCLS_VALUE && ( var->DataType==DT_CHAR ||  var->DataType==DT_VARCHAR ))
		{
			HDB_ASSERT(var->strval != NULL);
			//var->strval
			aggr->datatype = DT_CHAR;
		}
	
	}


	if(aggr->aggr_code==AGGR_SUM)
	{
		HDB_ASSERT(aggr->datatype==DT_NUMBER);
		aggr->numval += numval;
	}
	else if(aggr->aggr_code==AGGR_MAX)
	{
		if(aggr->datatype==DT_NUMBER)
		{
			if(aggr->process_count==0) // init
			{
				aggr->numval = numval;
			}
			else
			{
				aggr->numval = (aggr->numval > numval)?aggr->numval:numval;
			}
		}
		else if(aggr->datatype=DT_CHAR)
		{
			if(aggr->process_count==0) // init
			{
				if(aggr->strval!=NULL) free(aggr->strval);
				aggr->strval = (char*)malloc(strlen(var->strval)+1);
				strcpy(aggr->strval, var->strval);
			}
			else
			{
				if(strcmp(aggr->strval, var->strval) < 0)
				{
					if(aggr->strval!=NULL) free(aggr->strval);
					aggr->strval = (char*)malloc(strlen(var->strval)+1);
					strcpy(aggr->strval, var->strval);

				}
			}
		}
	}
	else if(aggr->aggr_code==AGGR_MIN)
	{
		if(aggr->datatype==DT_NUMBER)
		{
			if(aggr->process_count==0) // init
			{
				aggr->numval = numval;
			}
			else
			{
				aggr->numval = (aggr->numval < numval)?aggr->numval:numval;
			}
		}
		else if(aggr->datatype=DT_CHAR)
		{
			if(aggr->process_count==0) // init
			{
				if(aggr->strval!=NULL) free(aggr->strval);
				aggr->strval = (char*)malloc(strlen(var->strval)+1);
				strcpy(aggr->strval, var->strval);
			}
			else
			{
				if(strcmp(aggr->strval, var->strval) > 0)
				{
					if(aggr->strval!=NULL) free(aggr->strval);
					aggr->strval = (char*)malloc(strlen(var->strval)+1);
					strcpy(aggr->strval, var->strval);

				}
			}
		}
	}
	else if(aggr->aggr_code==AGGR_AVG)
	{
		HDB_ASSERT(aggr->datatype==DT_NUMBER);
		aggr->numval += numval; // devide when release
	}

	aggr->process_count++;

	if(var!=NULL) dDestroyVariable(var);

	return true;
}

char * dReleaseAggr(DBAggr * aggr, int * retcd)
{
	char * value;
	value=(char*)malloc(sizeof(char)*MAX_CHAR_LENGTH);
	value[0]=0;

	if(aggr->aggr_code==AGGR_AVG)
	{
		aggr->numval = aggr->numval / aggr->process_count; // devide when release
	}

	if(aggr->datatype==DT_NUMBER)
	{
		sprintf(value, "%f", aggr->numval);
	}
	else
	{
		strcpy(value, aggr->strval);
	}

	chop39(value);
	chop_lzero(value);

	*retcd = true;
	return value;
}

int dDestroyAggr(DBAggr * aggr)
{
	if(aggr->datatype==DT_UNKNOWN)
	{
		free(aggr->strval);
	}
	free(aggr);
	return true;
}

*/

