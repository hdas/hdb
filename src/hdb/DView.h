// DView.h: interface for the DView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DVIEW_H__991267E8_2AB1_49A3_B211_2EFAD7CBEBEA__INCLUDED_)
#define AFX_DVIEW_H__991267E8_2AB1_49A3_B211_2EFAD7CBEBEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "db.h"

class DView
{
public:
	DView();
	virtual ~DView();

	char name[TABLE_NAME_WIDTH];
};

#endif // !defined(AFX_DVIEW_H__991267E8_2AB1_49A3_B211_2EFAD7CBEBEA__INCLUDED_)
