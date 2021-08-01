// DReadableDatrix.h: interface for the DReadableDatrix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DREADABLEDATRIX_H__7686EDE4_2851_4485_B6FC_BFE2B10DAF2C__INCLUDED_)
#define AFX_DREADABLEDATRIX_H__7686EDE4_2851_4485_B6FC_BFE2B10DAF2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DDatrix.h"


class DReadableDatrix : public DDatrix
{
public:
	virtual int ReadSchema(DSchemaArray& schmArr) = 0;
	virtual int ReadData(DDataArray& dataArr) = 0;
	DReadableDatrix();
	virtual ~DReadableDatrix();

};

#endif // !defined(AFX_DREADABLEDATRIX_H__7686EDE4_2851_4485_B6FC_BFE2B10DAF2C__INCLUDED_)
