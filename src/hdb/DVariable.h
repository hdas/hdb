// DVariable.h: interface for the DVariable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(HD0293_DVARIABLE_H_INCLUDED)
#define HD0293_DVARIABLE_H_INCLUDED

#include "db.h"

class DVariable
{
public:
	char m_DataType;
	VarClass m_VarClass;
	// don't do following unsigned; some time it used to store direct return from function; 
	// so it may contain the error_code also
	int m_ref1; // may contains field_code
	int m_ref2; // may contains table index
	int m_ref3;
	int m_ref4;
	int m_ref5;
	char m_used;

	char * m_strval; // used for string
	double m_numval; // used for number/integer, boolean, or may be date

public:

	//static DVariable * PerformOparation(DVariable * vop1, DVariable * vop2, int oprtr, int *pRetCd);
	//static DVariable * eval_pf(DStack * pftlist, int *pRetCd);
	//static DVariable * eval_with_table(DExpr * the_expr_to_eval, DTable ** paTable, int nTable, int *pRetCd);
	//static DVariable * eval(char * stl[], int st, int lt, int *pRetCd);
	static DVariable * DVariable::CreateFieldVariable(char * pFieldName, DTable **paTables, int nTables, int *pRetcd);


	int Store(char * strval, int DataType);
	int ConvertTo(int NewDataType);
	char * GetStringValue();
	DVariable();
	DVariable::DVariable(const DVariable &Var);
	virtual ~DVariable();
	DVariable &DVariable::operator=(DVariable const &Var);

};

#endif // !defined(HD0293_DVARIABLE_H_INCLUDED)
