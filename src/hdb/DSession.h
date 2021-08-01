// DSession.h: interface for the DSession class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSESSION_H__4A30AD16_C9E5_44CE_808D_552B9A772C3D__INCLUDED_)
#define AFX_DSESSION_H__4A30AD16_C9E5_44CE_808D_552B9A772C3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DError.h"

class DSession
{
public:
	DSession(char * username);
	virtual ~DSession();

private:
	char m_username[TABLE_NAME_WIDTH];
	int m_hSock; // Contains the Connected Socket Handle
	char m_display; // Boolean : Used to inform the child function to display any result or not
	//DTable * pCacheTable[MAX_CACHE_TABLE];
	int m_table_lockid;
	DError * m_lastError;

public:
	char * GetUserName();
	int SetLastError(DError * pError);
	DError * GetLastError();
	int GetSockectHandle();
	void SetSockectHandle(int hSock);
	int Lock(DTable * pTable);
	int Unlock(DTable * pTable);
};

#endif // !defined(AFX_DSESSION_H__4A30AD16_C9E5_44CE_808D_552B9A772C3D__INCLUDED_)
