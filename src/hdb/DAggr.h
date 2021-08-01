// DAggr.h: interface for the DAggr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DAGGR_H__B958BE6E_DBE1_4FCB_9BA3_65B3CA080285__INCLUDED_)
#define AFX_DAGGR_H__B958BE6E_DBE1_4FCB_9BA3_65B3CA080285__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "db.h"
#include "DVariable.h"
#include "DExpr.h"
#include "DTable.h"

class DAggr
{
public:
	int DestroyAggr();
	char * ReleaseAggr(int * retcd);
	int ProcessAggr(DTable ** paTable, int nTable);
	int CreateAggr(char * aggr_name, DExpr * the_expr_in_aggr, int * retcd);
	DAggr();
	virtual ~DAggr();

	char m_aggr_name[TABLE_NAME_WIDTH];
	char m_aggr_code;
	int m_nExpr;  // Number of DBExpr in expr_arr
	DExpr * m_pExpr; // The Expr Array between Aggr i.e. in "sum(code+5)" "code+5" is in expr_arr
	int m_datatype;
	char * m_strval;
	double m_numval;
	int m_process_count;
};

#endif // !defined(AFX_DAGGR_H__B958BE6E_DBE1_4FCB_9BA3_65B3CA080285__INCLUDED_)
