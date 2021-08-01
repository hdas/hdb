// DTableDatrix.h: interface for the DTableDatrix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DTABLEDATRIX_H__BA3003C3_9E58_4D53_8493_A76ADF678F50__INCLUDED_)
#define AFX_DTABLEDATRIX_H__BA3003C3_9E58_4D53_8493_A76ADF678F50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DWriteableDatrix.h"
#include "DReadableDatrix.h"

class DTableDatrix :
	public DReadableDatrix,
	public DWriteableDatrix
{
public:
	DTableDatrix();
	virtual ~DTableDatrix();

};

#endif // !defined(AFX_DTABLEDATRIX_H__BA3003C3_9E58_4D53_8493_A76ADF678F50__INCLUDED_)
