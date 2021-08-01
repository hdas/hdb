#pragma once
#include "db.h"

class DError
{
public:
	DError(int errCode, char * errMsg = NULL, int errorType = 0, int nearToken = 0);
	~DError(void);
	int GetErrorCode();
private:
	int m_errCode;
	char * m_errMsg;
	int m_errorType;
	int m_nearToken;
};
