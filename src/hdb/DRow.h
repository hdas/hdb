// DRow.h: interface for the DRow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DROW_H__5659C9A6_1F5D_44CE_A77C_96FCA8D3DF23__INCLUDED_)
#define AFX_DROW_H__5659C9A6_1F5D_44CE_A77C_96FCA8D3DF23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DRow
{
public:
	DRow();
	virtual ~DRow();

	char ** m_arValues;
	DRow * m_NextRow;
};

#endif // !defined(AFX_DROW_H__5659C9A6_1F5D_44CE_A77C_96FCA8D3DF23__INCLUDED_)
