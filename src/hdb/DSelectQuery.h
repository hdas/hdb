// DSELECTQuery.h: interface for the DSELECTQuery class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSELECTQUERY_H__B546AFA3_F1AC_4542_A579_D1F9FB6EAEB7__INCLUDED_)
#define AFX_DSELECTQUERY_H__B546AFA3_F1AC_4542_A579_D1F9FB6EAEB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DConditionalQuery.h"
#include <vector>

using namespace std;

class DSELECTQuery : public DConditionalQuery
{
	friend class DQueryFactory;
private:
	DSELECTQuery(DSession *ssn, char * sql);
	virtual ~DSELECTQuery();

	int ComputeExprArray_Caption_And_Length(DExpr * expr_arr, int nCol, DTable * tbl, int nTbl);
	int DetermineExprArrayForAggr(DExpr * expr_arr, int nExpr);


public:
	int Parse();
	int Execute();

private:
	unsigned short m_nOutExpr; // Number of element in paExpr
	
	//DExpr ** m_paOutExpr; // Address of the Array of output Expression
	std::vector<DExpr *> m_paOutExpr;


	unsigned short m_nGroupByExpr; // Number of element in paGroupByExpr
	DExpr ** m_paGroupByExpr;

	unsigned short m_nOrderByExpr; // Number of element in paOrderByExpr
	DExpr ** m_paOrderByExpr;
};

#endif // !defined(AFX_DSELECTQUERY_H__B546AFA3_F1AC_4542_A579_D1F9FB6EAEB7__INCLUDED_)
