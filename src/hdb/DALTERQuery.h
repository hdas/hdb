// DALTERQuery.h: interface for the DALTERQuery class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DALTERQUERY_H__D6B161F2_8D87_4A90_9F22_2F105F945807__INCLUDED_)
#define AFX_DALTERQUERY_H__D6B161F2_8D87_4A90_9F22_2F105F945807__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DDDLQuery.h"

class DALTERQuery : public DDDLQuery
{
public:
	int Execute();
	int Parse();
	DALTERQuery(DSession *ssn, char * sql);
	virtual ~DALTERQuery();

};

#endif // !defined(AFX_DALTERQUERY_H__D6B161F2_8D87_4A90_9F22_2F105F945807__INCLUDED_)
