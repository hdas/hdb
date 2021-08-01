// DArray.cpp: implementation of the DArray class.
//
//////////////////////////////////////////////////////////////////////

#include "db.h"
#include "DArray.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DArray::DArray(int initialAllocSize, int increaseBy)
{
	m_storage = (void**)malloc(sizeof(void*) * initialAllocSize);
	m_iCurAlloc = initialAllocSize;
	m_iCurSize = 0;
	m_increaseBy = increaseBy;
}

DArray::~DArray()
{
	if (m_storage)
		free(m_storage);
}

int DArray::Add(void *ptr)
{
	if (m_iCurSize >= m_iCurAlloc)
	{
		m_iCurAlloc += m_increaseBy;
		m_storage = (void**)realloc(m_storage, sizeof(void*) * m_iCurAlloc);
	}

	m_storage[m_iCurSize++] = ptr;

	return TRUE;
}

int DArray::Remove(int index)
{
	return TRUE;
}
