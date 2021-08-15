// DVariable.cpp: implementation of the DVariable class.
//
//////////////////////////////////////////////////////////////////////

#include "DVariable.h"
#include "DStack.h"
#include "DExpr.h"
#include "DTable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DVariable::DVariable()
{
	m_DataType = DT_UNKNOWN;
	m_strval = NULL;
	m_VarClass = VarClass::Value;
	m_numval = 0;
	m_ref1 = 0;
	m_ref2 = 0;
	m_ref3 = 0;
	m_ref4 = 0;
	m_ref5 = 0;
	m_used = 0;
}

DVariable::DVariable(const DVariable &Var)
{
	//(*this) = Var;
	m_DataType = Var.m_DataType;
	m_VarClass = Var.m_VarClass;
	m_numval = Var.m_numval;
	m_ref1 = Var.m_ref1;
	m_ref2 = Var.m_ref2;
	m_ref3 = Var.m_ref3;
	m_ref4 = Var.m_ref4;
	m_ref5 = Var.m_ref5;
	m_used = Var.m_used;

	if (Var.m_strval)
	{
		m_strval = (char*)malloc(strlen(Var.m_strval) + 1);
		strcpy(m_strval, Var.m_strval);
	}
	else
		m_strval = NULL;
}

DVariable::~DVariable()
{
	if (m_strval)
		free(m_strval);
}

char * DVariable::GetStringValue()
{
	char * str;
	HDB_ASSERT(this);

	HDB_ASSERT(!(m_VarClass != VarClass::Value && m_VarClass != VarClass::Field));
	//if(var->m_VarClass != VCLS_VALUE && var->m_VarClass != VCLS_FIELD ) return NULL;

	//chop39(var->m_strval);
	//chop_lzero(var->m_strval);


	if (m_DataType == DT_NUMBER)
	{
		str = (char*)malloc(sizeof(char) * 20);
		sprintf(str, "%f", m_numval);
		chop_lzero(str);
	}
	else if (m_DataType == DT_BOOLEAN)
	{
		str = (char*)malloc(sizeof(char) * 2);
		if (m_numval) strcpy(str, "T"); else strcpy(str, "F");
	}
	else if (m_DataType == DT_CHAR || m_DataType == DT_VARCHAR)
	{
		str = (char*)malloc(sizeof(char) * (strlen(m_strval) + 1));
		strcpy(str, m_strval);
	}
	else if (m_DataType == DT_DATETIME)
	{
		str = (char*)malloc(sizeof(char) * 12);
		HDB_ASSERT(strlen(m_strval) < 10);
		strcpy(str, m_strval);
	}
	else
	{
		str = (char*)malloc(sizeof(char) * (strlen(m_strval) + 2));
		strcpy(str, m_strval);
		strcat(str, "*");
	}


	return str;
}

int DVariable::ConvertTo(int NewDataType)
{
	if (!m_strval)
		return FALSE;

	if (NewDataType == DT_NUMBER)
	{
		m_numval = atof(m_strval);
		free(m_strval);
		m_strval = NULL;
	}
	else if (NewDataType == DT_BOOLEAN)
	{
		if (EQUAL(m_strval, "1") || EQUAL(m_strval, "T") || EQUAL(m_strval, "TRUE"))
			m_numval = true;
		else
			m_numval = false;
		free(m_strval);
		m_strval = NULL;
	}
	else if ((NewDataType == DT_CHAR || NewDataType == DT_VARCHAR))
	{
		chop39(m_strval);
	}

	m_DataType = NewDataType;

	return true;
}

int DVariable::Store(char * strval, int DataType)
{
	if (m_strval)
		free(m_strval);
	m_strval = (char*)malloc(strlen(strval) + 1);
	strcpy(m_strval, strval);
	m_DataType = DataType;
	return true;
}

DVariable& DVariable::operator=(DVariable const &Var)
{
	m_DataType = Var.m_DataType;
	m_numval = Var.m_numval;
	m_VarClass = Var.m_VarClass;
	m_used = Var.m_used;
	m_ref1 = Var.m_ref1;
	m_ref2 = Var.m_ref2;
	m_ref3 = Var.m_ref3;
	m_ref4 = Var.m_ref4;
	m_ref5 = Var.m_ref5;

	if (Var.m_strval)
	{
		m_strval = (char*)malloc(strlen(Var.m_strval) + 1);
		strcpy(m_strval, Var.m_strval);
	}
	else
		m_strval = NULL;

	return *this;
}

// Looks like the function was depricated
/*DVariable * DVariable::eval(char *stl[], int st, int lt, int *pRetCd)
{
DVariable * pVar = NULL;
int res;
DStack * mystack=new DStack(), * pftlist=new DStack();

pftlist->PopulatePostfix(stl, pftlist, st, lt, NULL, 0);

// Reverse the stack into mystack
for(;;)
{
pVar = pftlist->Pop(); //dPopStack(pftlist, &res);
if(pftlist->m_errcd!=SUCCESS)
break;

res = mystack->Push(pVar, FALSE); // dPushStack(mystack, pVar);
//dDestroyVariable(pVar);
if(res != SUCCESS) break;
}

eval_pf(mystack, &res);

delete pftlist;
delete mystack;

return pVar;
}*/



DVariable * DVariable::CreateFieldVariable(char * pFieldName, DTable **paTables, int nTables, int *pRetcd)
{
	DVariable * vv = NULL;
	int i, tblidx = -1, fc = -1;

	for (i = 0; i < nTables; i++)
	{
		fc = paTables[i]->FieldCode(pFieldName);
		if (fc >= 0)
		{
			tblidx = i;
			break;
		}
	}

	if (tblidx >= 0) // i.e. Field Matched
	{
		vv = new DVariable(); //dCreateVariable(&res);
		vv->m_VarClass = VarClass::Field;
		vv->m_DataType = paTables[tblidx]->GetTagField(fc)->m_field_type;
		vv->m_ref1 = fc;
		vv->m_ref2 = tblidx;
		*pRetcd = 1;
	}
	else
	{
		*pRetcd = ERR_BADFIELD;
	}

	return vv;
}
