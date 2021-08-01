// DDropQuery.h: interface for the DDropQuery class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DDROPQUERY_H__2D67A406_9D2B_4326_885D_6FA191D7EB35__INCLUDED_)
#define AFX_DDROPQUERY_H__2D67A406_9D2B_4326_885D_6FA191D7EB35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DDDLQuery.h"

class DDROPQuery : public DDDLQuery
{
	friend class DQueryFactory;
private:
	DDROPQuery(DSession *ssn, char * sql);
	virtual ~DDROPQuery();

public:
	int Execute();
	int Parse();
};

#endif // !defined(AFX_DDROPQUERY_H__2D67A406_9D2B_4326_885D_6FA191D7EB35__INCLUDED_)
