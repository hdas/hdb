// DStack.cpp: implementation of the DStack class.
//
//////////////////////////////////////////////////////////////////////
#include "db.h"
#include "DStack.h"
#include "DTable.h"
#include "DVariable.h"

#include <vector>
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DStack::DStack()
{
	m_link = NULL;
	m_pVar = NULL;
}

DStack::~DStack()
{
	DVariable * pVar = this->Pop();
	while (pVar)
	{
		delete pVar;
		pVar = this->Pop();
	}
	// So, it is not destroying its member m_pVar
	// This is required bcoz, it is used to pop
}

int DStack::Push(DVariable * pVar, BOOL bCopyVar)
{
	DStack * TopStk = new DStack();
	TopStk->m_pVar = bCopyVar ? new DVariable(*pVar) : pVar;

	//alist->pVar->used++;

	TopStk->m_link = this->m_link;
	this->m_link = TopStk;
	m_errcd = SUCCESS;
	return m_errcd;
}

DVariable * DStack::Pop()
{
	DVariable * pVar;
	DStack * TopStk = m_link;

	if (TopStk == NULL)
	{
		m_errcd = ERR_EOF;
		return NULL;
	}

	pVar = TopStk->m_pVar; // should care that this is not destryed by destructor of DStack

	m_link = TopStk->m_link;

	TopStk->m_link = NULL;

	delete TopStk;

	//pVar->used--;
	m_errcd = SUCCESS;
	return pVar;
}

int DStack::PopulatePostfix(std::vector<char*> &tokens, DStack *pftlist, int st, int lt, DTable **paTable, int nTable)
{
	DVariable * cvar = NULL, *tpvar = NULL;
	int oprtr, retcd = false;
	int ti = st;
	//	int res;
	DStack * tmpstack = new DStack();

	while (ti <= lt)
	{
		cvar = new DVariable();
		HDB_ASSERT(tokens[ti] != NULL);
		oprtr = dGetOparatorCode(tokens[ti]);
		if (!oprtr) // Beacause oprtr is boolean (char); it returns false if bad oparator
		{
			cvar->Store(tokens[ti], DT_UNKNOWN); //dStoreVariable(cvar, DT_UNKNOWN, stl[ti]);
			cvar->m_VarClass = isValue(tokens[ti]) ? VCLS_VALUE : VCLS_FIELD;
			if (cvar->m_VarClass == VCLS_VALUE)
			{
				if (tokens[ti][0] == 39)
					cvar->ConvertTo(DT_CHAR); //dChangeVariableDatatype(cvar, DT_CHAR);
				else
					cvar->ConvertTo(DT_NUMBER); //dChangeVariableDatatype(cvar, DT_NUMBER);
			}
			else // VCLS_FIELD ; so, compute the field reference
			{
				int ti = 0;
				cvar->m_ref1 = -1;
				for (ti = 0; ti < nTable; ti++)
				{
					cvar->m_ref1 = paTable[ti]->FieldCode(cvar->m_strval);
					//if(cvar->ref1<0) HDB_RETURN(cvar->ref1); // don't do it here
					if (cvar->m_ref1 >= 0)
					{
						cvar->m_DataType = paTable[ti]->FieldType(cvar->m_ref1);
						cvar->m_ref2 = ti;
						break;
					}
				}
				if (cvar->m_ref1 < 0) HDB_RETURN(ERR_BADFIELD);
			}
		}
		else
		{
			cvar->m_VarClass = VCLS_OPARATOR;
			cvar->m_ref1 = oprtr;
			if (ti == st)
			{
				if (EQUAL(tokens[ti], "-")) cvar->m_ref1 = OP_UMINUS;
				if (EQUAL(tokens[ti], "+")) cvar->m_ref1 = OP_UPLUS;
			}
		}

		if (cvar->m_VarClass != VCLS_OPARATOR) // Current token is a bad oparator i.e. it is a oparand
		{
			pftlist->Push(cvar, FALSE);
			cvar = NULL;
		}
		else if (cvar->m_ref1 == OP_BRACSTOP) // ")"
		{
			tpvar = tmpstack->Pop();
			if (tmpstack->m_errcd != SUCCESS)
				HDB_RETURN(ERR_BADEXPR);

			while (tpvar->m_ref1 != OP_BRACSTART)
			{
				pftlist->Push(tpvar, FALSE);
				tpvar = tmpstack->Pop();
				if (tmpstack->m_errcd != SUCCESS)
					HDB_RETURN(ERR_BADEXPR);
			} // actually token "(" has come out from the stack adn it is that
		}
		else  // oparator or ')'
		{
			tpvar = tmpstack->Pop();
			if (tmpstack->m_errcd == SUCCESS)
			{
				while (isp(tpvar->m_ref1) >= icp(cvar->m_ref1))
				{
					pftlist->Push(tpvar, FALSE);
					tpvar = tmpstack->Pop();
					if (tmpstack->m_errcd != SUCCESS)
						break;
				}
				if (tmpstack->m_errcd != SUCCESS)
					tmpstack->Push(tpvar);// push again tptkn ind free it
			}
			tmpstack->Push(cvar, FALSE);
			cvar = NULL;
		}

		if (cvar)
			delete cvar;
		ti++;
	} // of while

	// end of expression; empty stack
	for (;;)
	{
		tpvar = tmpstack->Pop();
		if (tmpstack->m_errcd != SUCCESS)
			break;
		pftlist->Push(tpvar, FALSE); // When using FALSE no need to destroy
		//dDestroyVariable(tpvar);
	}
	retcd = true;

CLEANUP:
	if (tmpstack)
		delete tmpstack;

	return retcd;
}
