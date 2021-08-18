// DConditionalQuery.h: interface for the DConditionalQuery class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCONDITIONALQUERY_H__9DD9E3C8_5CAF_4CCD_9089_9A6854B2BBDC__INCLUDED_)
#define AFX_DCONDITIONALQUERY_H__9DD9E3C8_5CAF_4CCD_9089_9A6854B2BBDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DQuery.h"
#include <vector>

class DConditionalQuery : public DQuery
{
public:
	int OpenTableList(int startTokenTable);
	int ParseTableList();
	int ParseCondition(int st_where, int nd_where = 0);
	DConditionalQuery(DSession *ssn, char * sql);
	virtual ~DConditionalQuery();

	virtual int Parse() = 0;
	virtual int Execute() = 0;

protected:
	//unsigned short m_nTable; // Number of element in paTable
	std::vector<DTable *> m_paTable;

	unsigned short m_nWhereExpr; // Number of Where Expr; its values is either 0 or 1
	DExpr * m_pWhereExpr; // Address of the Where Expression; 1 element


};

#endif // !defined(AFX_DCONDITIONALQUERY_H__9DD9E3C8_5CAF_4CCD_9089_9A6854B2BBDC__INCLUDED_)
