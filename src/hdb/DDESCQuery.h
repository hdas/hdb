// DDESCQuery.h: interface for the DDESCQuery class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DDESCQUERY_H__707014C1_DAC7_4DD3_80D0_ED542B1CA3A0__INCLUDED_)
#define AFX_DDESCQUERY_H__707014C1_DAC7_4DD3_80D0_ED542B1CA3A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DQuery.h"

class DDESCQuery : public DQuery
{
	friend class DQueryFactory;
private:
	DDESCQuery(DSession *ssn, char * sql);
	virtual ~DDESCQuery();

public:
	int Execute();
	int Parse();
};

#endif // !defined(AFX_DDESCQUERY_H__707014C1_DAC7_4DD3_80D0_ED542B1CA3A0__INCLUDED_)
