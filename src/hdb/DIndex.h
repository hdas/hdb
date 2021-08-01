// DIndex.h: interface for the DIndex class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DINDEX_H__905652E0_069C_47BA_98A3_A8447EF10250__INCLUDED_)
#define AFX_DINDEX_H__905652E0_069C_47BA_98A3_A8447EF10250__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "db.h"

class DIndex
{
public:
	DIndex();
	virtual ~DIndex();

	char idxname[TABLE_NAME_WIDTH];
	char tname[TABLE_NAME_WIDTH];
	char isUnique;
	FILE * fp;
	tagDBField * field;
	int nFields;
};

#endif // !defined(AFX_DINDEX_H__905652E0_069C_47BA_98A3_A8447EF10250__INCLUDED_)
