// DDMLQuery.h: interface for the DDmlQuery class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DDMLQUERY_H__6E3B9FD9_4174_4AEB_B59D_426ABAEA6DB8__INCLUDED_)
#define AFX_DDMLQUERY_H__6E3B9FD9_4174_4AEB_B59D_426ABAEA6DB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DConditionalQuery.h"

class DDMLQuery : public DConditionalQuery
{
protected:
	DDMLQuery(DSession *ssn, char * sql);
	virtual ~DDMLQuery();

	unsigned short m_nVarToSet; // Number of Variable ( must be VCLS_FIELD )
	DVariable ** m_paVarToSet; // Address of the Array of DBVariable(s)

	unsigned short m_nInExpr; // Number of InputExpr
	DExpr ** m_paInExpr;

};

#endif // !defined(AFX_DDMLQUERY_H__6E3B9FD9_4174_4AEB_B59D_426ABAEA6DB8__INCLUDED_)
