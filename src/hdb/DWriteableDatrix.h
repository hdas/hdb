// DWriteableDatrix.h: interface for the DWriteableDatrix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DWRITEABLEDATRIX_H__5B3AC93A_AEC9_4109_8C13_913645DA6386__INCLUDED_)
#define AFX_DWRITEABLEDATRIX_H__5B3AC93A_AEC9_4109_8C13_913645DA6386__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "db.h"
#include "DDatrix.h"

class DWriteableDatrix : public DDatrix
{
public:
	virtual int WriteData(const DDataArray& dataArr, BOOL bEndData) = 0;
	virtual int WriteSchema(const DSchemaArray& schmArr, BOOL bEndSchm) = 0;
	DWriteableDatrix();
	virtual ~DWriteableDatrix();

};

#endif // !defined(AFX_DWRITEABLEDATRIX_H__5B3AC93A_AEC9_4109_8C13_913645DA6386__INCLUDED_)
