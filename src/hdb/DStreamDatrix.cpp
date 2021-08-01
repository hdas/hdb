// DStreamDatrix.cpp: implementation of the DStreamDatrix class.
//
//////////////////////////////////////////////////////////////////////

#include "DStreamDatrix.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DStreamDatrix::DStreamDatrix()
{
	m_outDatrix = NULL;
}

DStreamDatrix::~DStreamDatrix()
{

}

///
/// Have to arrange the cleanup mechanism of outDatrix
///
int DStreamDatrix::AttachOutputDatrix(DWriteableDatrix *outDatrix)
{
	m_outDatrix = outDatrix;
	return TRUE;
}

int DStreamDatrix::WriteData(const DDataArray &dataArr, BOOL bEndData)
{
	if (m_outDatrix)
		return m_outDatrix->WriteData(dataArr, bEndData);
	else
		return FALSE;
}

int DStreamDatrix::WriteSchema(const DSchemaArray &schmArr, BOOL bEndSchm)
{
	if (m_outDatrix)
		return m_outDatrix->WriteSchema(schmArr, bEndSchm);
	else
		return FALSE;
}
