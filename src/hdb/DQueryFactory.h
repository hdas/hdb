// DQueryFactory.h: interface for the DQueryFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DQUERYFACTORY_H__F8CB183F_7A6C_4C3D_A5A0_94F2856F529A__INCLUDED_)
#define AFX_DQUERYFACTORY_H__F8CB183F_7A6C_4C3D_A5A0_94F2856F529A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "db.h"

class DQueryFactory
{
public:
	int m_errcd;
	char m_errmsg[MAX_PATH];
	static DQuery * CreateQuery(DSession * ssn, char * sql);
	DQueryFactory();
	virtual ~DQueryFactory();

};

#endif // !defined(AFX_DQUERYFACTORY_H__F8CB183F_7A6C_4C3D_A5A0_94F2856F529A__INCLUDED_)
