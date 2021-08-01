// DVar.h: interface for the DVar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DVAR_H__15091C89_B489_45F2_9E2D_63DD8E2CE329__INCLUDED_)
#define AFX_DVAR_H__15091C89_B489_45F2_9E2D_63DD8E2CE329__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DVar
{
public:
	enum DDataType
	{
		DT_NUMBER = 'N',
		DT_INTEGER = 'I',

		DT_DATE = 'D',
		DT_TIME = 'T',
		DT_DATETIME = 'A',

		DT_VARCHAR = 'V',
		DT_VARCHAR2 = 'X',
		DT_CHAR = 'C',

		DT_BOOLEAN = 'B',
		DT_BLOB = 'L',

		DT_FIELDREF = 'f',
		DT_DATRIXREF = 'd',
		DT_QUERYREF = 'q',
		DT_PARAMREF = 'p'
	};

	DVar();
	virtual ~DVar();
	inline DDataType GetDataType(){ return m_datatype; };

	virtual char * CreateStringValue() = 0;

private:
	DDataType m_datatype;

};

#endif // !defined(AFX_DVAR_H__15091C89_B489_45F2_9E2D_63DD8E2CE329__INCLUDED_)
