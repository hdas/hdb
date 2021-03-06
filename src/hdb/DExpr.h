// DExpr.h: interface for the DExpr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEXPR_H__6B9F3861_09AE_43C1_ACEA_9C6E691BAE7D__INCLUDED_)
#define AFX_DEXPR_H__6B9F3861_09AE_43C1_ACEA_9C6E691BAE7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "db.h"
#include "DVariable.h"
#include "DStack.h"
#include "DTable.h"

class DExpr
{
private:
	//int m_errcd;
	char * m_caption;
	unsigned short m_length;
	unsigned short m_nAggr;
	DStack * m_pExprStack;
	int m_nTable;
	DVariable * EvalPostfix(DStack * pftlist);
	int Create(std::vector<DTable *> &paTable, std::vector<char*> &tokens, std::pair<short, short> &range);

public:
	static DVariable * PerformOparation(DVariable *vop1, DVariable *vop2, int oprtr, int * pRetCd);

	DExpr(std::vector<DTable *> &paTable, std::vector<char*> &tokens, std::pair<short, short> &range);
	virtual ~DExpr();

	int GetLength();
	char * GetCaption();
	DVariable * Evaluate(std::vector<DTable *> &paTable);
};

#endif // !defined(AFX_DEXPR_H__6B9F3861_09AE_43C1_ACEA_9C6E691BAE7D__INCLUDED_)
