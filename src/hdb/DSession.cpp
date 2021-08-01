// DSession.cpp: implementation of the DSession class.
//
//////////////////////////////////////////////////////////////////////

#include "DSession.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DSession::DSession(char * username)
{
	this->m_display = TRUE;
	this->m_hSock = -1;
	this->m_table_lockid = 1;
	strcpy(this->m_username, username);
	this->m_lastError = NULL;

	/*for(int i=0; i<MAX_CACHE_TABLE; i++)
		this->pCacheTable[i] = NULL;*/
}

DSession::~DSession()
{
	/*for(int i=0; i<MAX_CACHE_TABLE; i++)
	{
	if(this->pCacheTable[i])
	delete pCacheTable[i]; // Closes the opened table
	}*/
}

char * DSession::GetUserName()
{
	return m_username;
}

int DSession::SetLastError(DError * pError)
{
	if (m_lastError != NULL)
	{
		delete m_lastError;
	}
	m_lastError = pError;
	return 0;
}

DError * DSession::GetLastError()
{
	return m_lastError;
}

int DSession::GetSockectHandle()
{
	return m_hSock;
}

void DSession::SetSockectHandle(int hSock)
{
	m_hSock = hSock;
}

int DSession::Lock(DTable * pTable)
{
	return SUCCESS;
}

int DSession::Unlock(DTable * pTable)
{
	return SUCCESS;
}