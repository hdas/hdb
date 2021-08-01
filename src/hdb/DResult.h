// DResult.h: interface for the DResult class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRESULT_H__B49D5BB3_03EB_41C4_9EAE_E11F8B39297A__INCLUDED_)
#define AFX_DRESULT_H__B49D5BB3_03EB_41C4_9EAE_E11F8B39297A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "db.h"
#include "DExpr.h"

class DResult
{
private:
	int Create();
	int Close();
public:
	int SortResult(int * iSortOnArr, int nSortOnArr, int Direction);
	DRow * AddRow();
	int Output();
	DResult();
	virtual ~DResult();

	int m_errcd;
	char m_errmsg[256];
	unsigned int m_nCol;
	unsigned int m_nRow;
	char m_SqlType;
	tagDBField * m_FieldList;
	DRow * m_FirstRow; // first arValue is NULL
	DRow * m_cRow; // Stores the link of current row; used for temp processing purpose

};

#endif // !defined(AFX_DRESULT_H__B49D5BB3_03EB_41C4_9EAE_E11F8B39297A__INCLUDED_)
