// DQuery.h: interface for the DQuery class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DQUERY_H__984185DD_A642_46AD_AF0D_478FB0838632__INCLUDED_)
#define AFX_DQUERY_H__984185DD_A642_46AD_AF0D_478FB0838632__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "db.h"
#include "DRow.h"
#include "DResult.h"
#include "DTable.h"
#include "DVariable.h"
#include "DExpr.h"
#include "DError.h"
#include "DSession.h"

#include <vector>
using namespace std;

class DQuery
{
public:
	DResult * CreateResult();
	DResult * GetResult();

	virtual int Parse() = 0;
	virtual int Execute() = 0;


	int GetErrorCode();
	char * GetErrorMessage();

	DQuery(DSession *ssn, char * sql);
	virtual ~DQuery();

protected:
	BOOL m_bParsed;
	BOOL m_bExecuted;
	BOOL m_bDestroyResult;

	int m_errcd;
	char m_errmsg[MAX_PATH];
	DResult * m_result;

	DSession * m_ssn;

	int m_nToken;
	char m_SqlType;

	// deprecated, user m_tokens
	char ** m_stl; // SQL Token List 

	std::vector<char*> m_tokens;
	
	//	DBVariable * vtl; // DBVariable Token List; it is a parsed version of stl

	//	int nFields;
	//	DBField * FieldList;
	//	int nTable;
	//	DBTable * TableList;
};

#endif // !defined(AFX_DQUERY_H__984185DD_A642_46AD_AF0D_478FB0838632__INCLUDED_)
