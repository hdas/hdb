// DInsertQuery.h: interface for the DInsertQuery class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DINSERTQUERY_H__C4593460_CF92_4B06_87ED_F0DD409BCB5E__INCLUDED_)
#define AFX_DINSERTQUERY_H__C4593460_CF92_4B06_87ED_F0DD409BCB5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DDMLQuery.h"

class DINSERTQuery : public DDMLQuery
{
	friend class DQueryFactory;
private:
	int CheckConstraints(DTable * tbl, int nTbl);
	DINSERTQuery(DSession *ssn, char * sql);
	virtual ~DINSERTQuery();

public:
	int Execute();
	int Parse();
};

#endif // !defined(AFX_DINSERTQUERY_H__C4593460_CF92_4B06_87ED_F0DD409BCB5E__INCLUDED_)
