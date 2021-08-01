// DMemDatrix.h: interface for the DMemDatrix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DMEMDATRIX_H__68E1B5C4_E0A6_4543_A4A0_1A973116BB65__INCLUDED_)
#define AFX_DMEMDATRIX_H__68E1B5C4_E0A6_4543_A4A0_1A973116BB65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DWriteableDatrix.h"
#include "DReadableDatrix.h"

class DMemDatrix :
	public DReadableDatrix,
	public DWriteableDatrix
{
public:
	DMemDatrix();
	virtual ~DMemDatrix();

};

#endif // !defined(AFX_DMEMDATRIX_H__68E1B5C4_E0A6_4543_A4A0_1A973116BB65__INCLUDED_)
