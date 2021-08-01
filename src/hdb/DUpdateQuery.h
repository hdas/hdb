// DUpdateQuery.h: interface for the DUpdateQuery class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DUPDATEQUERY_H__BC424DA4_160F_4C8A_847A_6E407D0FF7F1__INCLUDED_)
#define AFX_DUPDATEQUERY_H__BC424DA4_160F_4C8A_847A_6E407D0FF7F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DDMLQuery.h"

class DUPDATEQuery : public DDMLQuery
{
	friend class DQueryFactory;
private:
	int CreateUpdateQuery();
	DUPDATEQuery(DSession *ssn, char * sql);
	virtual ~DUPDATEQuery();

public:
	int Execute();
	int Parse();

};

#endif // !defined(AFX_DUPDATEQUERY_H__BC424DA4_160F_4C8A_847A_6E407D0FF7F1__INCLUDED_)
