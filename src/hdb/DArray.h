// DArray.h: interface for the DArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DARRAY_H__911FAC15_9AD1_4F64_9646_942F5E804767__INCLUDED_)
#define AFX_DARRAY_H__911FAC15_9AD1_4F64_9646_942F5E804767__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DArray
{
public:
	virtual int Remove(int index);
	virtual int Add(void * ptr);
	DArray(int initialAllocSize = 2, int increaseBy = 5);
	virtual ~DArray();
private:
	void ** m_storage;
	int m_iCurAlloc;
	int m_iCurSize;
	int m_increaseBy;
};

#endif // !defined(AFX_DARRAY_H__911FAC15_9AD1_4F64_9646_942F5E804767__INCLUDED_)
