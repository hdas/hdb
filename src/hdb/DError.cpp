#include "DError.h"

DError::DError(int errCode, char * errMsg, int errorType, int nearToken)
{
	this->m_errCode = errCode;
	if (errMsg != NULL)
	{
		this->m_errMsg = new char[strlen(errMsg) + 1];
		strcpy(this->m_errMsg, errMsg);
	}
	this->m_errorType = errorType;
	this->m_nearToken = nearToken;
}

DError::~DError(void)
{
	if (this->m_errMsg != NULL)
		delete[] this->m_errMsg;
}

int DError::GetErrorCode()
{
	return m_errCode;
}