// DCreateQuery.h: interface for the DCreateQuery class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCREATEQUERY_H__41ADDDEC_389B_46C0_96D5_E73D735E470D__INCLUDED_)
#define AFX_DCREATEQUERY_H__41ADDDEC_389B_46C0_96D5_E73D735E470D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DDDLQuery.h"

class DCREATEQuery : public DDDLQuery
{
	friend class DQueryFactory;
private:
	DCREATEQuery(DSession *ssn, char * sql);
	virtual ~DCREATEQuery();

public:
	int ExecuteCreateTable(BOOL bCheckCat = TRUE);
	int ExecuteCreateIndex();
	int Execute();
	int Parse();
};

#endif // !defined(AFX_DCREATEQUERY_H__41ADDDEC_389B_46C0_96D5_E73D735E470D__INCLUDED_)
