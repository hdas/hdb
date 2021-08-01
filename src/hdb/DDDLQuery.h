// DDDLQuery.h: interface for the DDDLQuery class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DDDLQUERY_H__D1494998_93E9_4E49_B1F3_936955977959__INCLUDED_)
#define AFX_DDDLQUERY_H__D1494998_93E9_4E49_B1F3_936955977959__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DQuery.h"

class DDDLQuery : public DQuery
{
public:
	DDDLQuery(DSession *ssn, char * sql);
	virtual ~DDDLQuery();
	//virtual int Parse();
	//virtual int Execute();
};

#endif // !defined(AFX_DDDLQUERY_H__D1494998_93E9_4E49_B1F3_936955977959__INCLUDED_)
