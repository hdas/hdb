// DStack.h: interface for the DStack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSTACK_H__14C4A12B_E198_4C64_A14D_1E0EDA9E17B8__INCLUDED_)
#define AFX_DSTACK_H__14C4A12B_E198_4C64_A14D_1E0EDA9E17B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "db.h"

class DStack
{
public:
	int PopulatePostfix(std::vector<char*> &tokens, DStack * pftlist, int st, int lt, DTable ** paTable, int nTable);
	DVariable * Pop();
	int Push(DVariable * pVar, BOOL bCopyVar = TRUE);
	DStack();
	virtual ~DStack();
	//char * tkn;
	DVariable * m_pVar;
	DStack * m_link;
	int m_errcd;
};

#endif // !defined(AFX_DSTACK_H__14C4A12B_E198_4C64_A14D_1E0EDA9E17B8__INCLUDED_)
