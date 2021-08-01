// DQuery.cpp: implementation of the DQuery class.
//
//////////////////////////////////////////////////////////////////////

#include "DQuery.h"
#include "Token.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DQuery::DQuery(DSession *ssn, char * sql)
{
	char * tt;
	int i;

	//initialize the variables
	m_bParsed = FALSE;
	m_bExecuted = FALSE;
	m_bDestroyResult = TRUE;
	m_errmsg[0] = NULL;

	m_ssn = ssn;

	m_result = new DResult();

	m_nToken = 0;
	for (;;)
	{
		tt = CreateToken(sql, m_nToken++);
		if (EQUAL(tt, ""))
		{
			free(tt);
			break;
		}
		else
		{
			m_tokens.push_back(tt);
		}
	}

	m_nToken = m_tokens.size();

	m_stl = (char**)malloc(sizeof(char*) * m_nToken);
	HDB_ASSERT(m_stl != NULL);

	for (i = 0; i < m_nToken; i++)
	{
		m_stl[i] = m_tokens[i]; // CreateToken(sql, i);
	}
	m_errcd = SUCCESS;
}

DQuery::~DQuery()
{
	int i;

	for (i = 0; i < m_tokens.size(); i++)
	{
		if (m_tokens[i])
		{
			free(m_tokens[i]);
			m_tokens[i] = NULL;
		}
	}
	m_tokens.clear();

	if (m_stl)
	{
		free(m_stl);
		m_stl = NULL;
	}

	if (m_bDestroyResult && m_result)
	{
		delete m_result;
		m_result = NULL;
	}
}

DResult * DQuery::GetResult()
{
	return m_result;
}


DResult * DQuery::CreateResult()
{
	m_bDestroyResult = FALSE;
	return m_result;
}
