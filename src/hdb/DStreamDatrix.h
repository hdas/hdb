// DStreamDatrix.h: interface for the DStreamDatrix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSTREAMDATRIX_H__246973F3_0244_4765_9CDC_13E636058EE0__INCLUDED_)
#define AFX_DSTREAMDATRIX_H__246973F3_0244_4765_9CDC_13E636058EE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DWriteableDatrix.h"

class DStreamDatrix : public DWriteableDatrix
{
public:
	virtual int WriteSchema(const DSchemaArray& schmArr, BOOL bEndSchm);
	virtual int WriteData(const DDataArray& dataArr, BOOL bEndData);
	int AttachOutputDatrix(DWriteableDatrix * outDatrix);
	DStreamDatrix();
	virtual ~DStreamDatrix();
private:
	DWriteableDatrix * m_outDatrix;

};

#endif // !defined(AFX_DSTREAMDATRIX_H__246973F3_0244_4765_9CDC_13E636058EE0__INCLUDED_)
