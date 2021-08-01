// DDeleteQuery.h: interface for the DDeleteQuery class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DDELETEQUERY_H__65E1076C_A0D4_458F_A738_D8774DF2907B__INCLUDED_)
#define AFX_DDELETEQUERY_H__65E1076C_A0D4_458F_A738_D8774DF2907B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DDMLQuery.h"

class DDELETEQuery : public DDMLQuery
{
	friend class DQueryFactory;
private:
	DDELETEQuery(DSession *ssn, char * sql);
	virtual ~DDELETEQuery();
public:
	int Parse();
	int Execute();
};

#endif // !defined(AFX_DDELETEQUERY_H__65E1076C_A0D4_458F_A738_D8774DF2907B__INCLUDED_)
