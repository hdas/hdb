// DExpr.cpp: implementation of the DExpr class.
//
//////////////////////////////////////////////////////////////////////

#include "DExpr.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DExpr::DExpr(DTable ** paTable, int nTable)
{
	this->m_pExprStack = NULL;
	this->m_caption = NULL;
	this->m_length = 0;
	this->m_nAggr = 0;
	this->m_paTable = paTable;
	this->m_nTable = nTable;
}

char * DExpr::GetCaption()
{
	return m_caption;
}

int DExpr::GetLength()
{
	return m_length;
}

int DExpr::Create(std::vector<char*> &tokens, int startTokenIdx, int endTokenIdx)
{
	DExpr * expr = NULL;
	int retcd = true, res;
	DStack * rvstk = new DStack(), *pfstk = new DStack();
	DVariable * pVar = NULL;

	res = pfstk->PopulatePostfix(tokens, pfstk, startTokenIdx, endTokenIdx, m_paTable, m_nTable);
	if (res != SUCCESS)
	{
		HDB_RETURN(res);
	}

	// Reverse the stack into rvstk
	for (;;)
	{
		pVar = pfstk->Pop();
		if (pfstk->m_errcd != SUCCESS)
		{
			if (pfstk->m_errcd == ERR_EOF)
				break;
			else
				HDB_RETURN(res);
		}

		res = rvstk->Push(pVar, FALSE);

		if (rvstk->m_errcd != SUCCESS)
			HDB_RETURN(res);
	}

	// Now create & filling the Expr
	m_pExprStack = rvstk;
	m_caption = (char*)malloc(strlen(tokens[startTokenIdx]) + 1);
	strcpy(m_caption, tokens[startTokenIdx]);

	m_length = 9;
	if (startTokenIdx == endTokenIdx)
	{
		if (m_pExprStack->m_link->m_pVar->m_VarClass == VCLS_FIELD)
		{
			m_length = m_paTable[m_pExprStack->m_link->m_pVar->m_ref2]->GetTagField(m_pExprStack->m_link->m_pVar->m_ref1)->m_field_length;
		}
		else
		{
			if (m_pExprStack->m_link->m_pVar->m_VarClass == VCLS_VALUE)
			{
				if (m_pExprStack->m_link->m_pVar->m_DataType == DT_CHAR)
				{
					m_length = strlen(m_pExprStack->m_link->m_pVar->m_strval);
				}
				else if (m_pExprStack->m_link->m_pVar->m_DataType == DT_DATETIME)
				{
					m_length = 10;
				}
			}
		}
	}

	retcd = true;

CLEANUP:
	delete pfstk;
	return retcd;
}

DExpr::~DExpr()
{
	if (m_caption)
		free(m_caption);

	if (m_pExprStack)
		delete m_pExprStack;
}


DVariable * DExpr::Evaluate(int * pRetCd)
{
	int res = FALSE, retcd = TRUE; //, fld_typ;
	DVariable * pVar = NULL;
	DStack * tmplink = NULL;


	tmplink = this->m_pExprStack;

	// many change to do
	// mainly filling values of variables; this is done by following loop

	// Note : the stack is already postfix ed and reversed; so don't alter stack sequence
	for (;;) // the pVar contains only reference; so don't destroy it here; it will used for next record also
	{
		tmplink = tmplink->m_link; if (tmplink == NULL) break;
		pVar = tmplink->m_pVar; HDB_ASSERT(pVar != NULL);

		if (pVar->m_VarClass == VCLS_FIELD) // Just store the value from table
		{
			int typ = DT_UNKNOWN;
			if (pVar->m_strval != NULL) free(pVar->m_strval);
			typ = pVar->m_DataType; // backup; storing
			pVar->m_DataType = DT_UNKNOWN;
			pVar->m_strval = m_paTable[pVar->m_ref2]->FetchField(pVar->m_ref1, &res);
			if (res != 1) HDB_RETURN(res);
			pVar->ConvertTo(typ); //dChangeVariableDatatype(pVar, typ); 
		}
	}

	if (this->m_pExprStack->m_link->m_link == NULL) // The expr contains only one value; field
	{
		pVar = new DVariable(*(this->m_pExprStack->m_link->m_pVar));

		//(*pVar) = (*(the_expr_to_eval->m_pExprStack->m_link->m_pVar));

		//dCreateDuplicateVariable(the_expr_to_eval->m_pExprStack->m_link->m_pVar, &res);
	}
	else
	{
		pVar = this->EvalPostfix(this->m_pExprStack, &retcd);
	}

CLEANUP:
	*pRetCd = retcd;
	return pVar;

}

DVariable * DExpr::EvalPostfix(DStack * pftlist, int * pRetCd)
{
	DVariable * vtt = NULL, *vop1 = NULL, *vop2 = NULL, *vrst = NULL;
	int logic_res = true, res = true;
	char bError = false;
	int oprtr;
	DStack * mystack = new DStack(), *tmplink = NULL;

	tmplink = pftlist;

	while (res == true && bError != true)
	{
		tmplink = tmplink->m_link;
		if (tmplink == NULL) break;
		vtt = tmplink->m_pVar; // Just take the reference; because it will not destroy in future; it is used for next record also 
		HDB_ASSERT(vtt != NULL);

		if (vtt->m_VarClass == VCLS_OPARATOR)
		{
			oprtr = vtt->m_ref1;
			if (!(oprtr == OP_NOT || oprtr == OP_UPLUS || oprtr == OP_UMINUS))
			{
				vop2 = mystack->Pop(); // dPopStack(mystack, &res);
				if (mystack->m_errcd != SUCCESS)
					bError = true;
			}
			else
				vop2 = NULL;

			vop1 = mystack->Pop(); //dPopStack(mystack, &res);
			if (mystack->m_errcd != SUCCESS)
				bError = true;

			if (!bError)
			{
				vrst = PerformOparation(vop1, vop2, oprtr, &logic_res);
				if (mystack->m_errcd != SUCCESS)
					bError = true;
			}

			if (!bError)
			{
				HDB_ASSERT(vrst != NULL);
				mystack->Push(vrst, FALSE); // dPushStack(mystack, vrst);
				if (mystack->m_errcd != SUCCESS)
					bError = true;
				//dDestroyVariable(vrst);
			}
			//dDestroyVariable(vtt);
			delete vop1;
			delete vop2;
		}
		else
			mystack->Push(vtt, TRUE);

		//free(tt);
		//dDestroyVariable(vtt);
		//vtt = dPopStack(pftlist, &res);
	} // of while(res == true && error!=true)
	//free(tt);
	//dDestroyVariable(vtt);

	// ********************** VVI Note : don't destroy vtt here; it is only a reference

	if (!bError)
	{
		vtt = mystack->Pop();
		if (mystack->m_errcd == SUCCESS)
		{
			*pRetCd = true;
			delete mystack;
			return vtt;
		}
	}

	if (logic_res == 1)
		logic_res = ERR_BADEXPR;

	*pRetCd = logic_res;
	delete mystack;
	return NULL;
}

DVariable * DExpr::PerformOparation(DVariable *vop1, DVariable *vop2, int oprtr, int * pRetCd)
{
	DVariable * vrst = NULL;
	int cmp, retcd = true; // res_datatype=0;
	char str_res[255] = "";

	HDB_ASSERT(vop1 != NULL);

	if (vop1->m_VarClass != VCLS_VALUE && vop1->m_VarClass != VCLS_FIELD) HDB_RETURN(ERR_BADEXPR);
	// Change to common/generic datatype
	if (vop1->m_DataType == DT_VARCHAR) vop1->m_DataType = DT_CHAR;

	// some oparation has only one oparand; so following
	if (oprtr == OP_UMINUS || oprtr == OP_UPLUS || oprtr == OP_NOT)
	{
		if (vop2 != NULL) HDB_RETURN(ERR_BADEXPR);
	}
	else
	{
		HDB_ASSERT(vop2 != NULL);
		if (vop2->m_VarClass != VCLS_VALUE && vop2->m_VarClass != VCLS_FIELD) HDB_RETURN(ERR_BADEXPR);

		if (vop2->m_DataType == DT_VARCHAR) vop2->m_DataType = DT_CHAR;
		if (vop1->m_DataType != vop2->m_DataType) HDB_RETURN(ERR_DATATYPEMISMATCH);
	}



	vrst = new DVariable(); //dCreateVariable(&res);

	switch (oprtr)
	{
	case OP_PLUS:
	{
		//sprintf(str_res,"%f",vop1->m_numval + vop2->m_numval);
		//res_datatype = DT_NUMBER;
		vrst->m_DataType = DT_NUMBER;
		vrst->m_numval = vop1->m_numval + vop2->m_numval;
		break;
	}
	case OP_UPLUS:
	{
		//sprintf(str_res,"%f",0 + vop1->m_numval);
		vrst->m_DataType = DT_NUMBER;
		vrst->m_numval = vop1->m_numval;

		break;
	}
	case OP_UMINUS:
	{
		//sprintf(str_res,"%f",0 - vop1->m_numval);
		vrst->m_DataType = DT_NUMBER;
		vrst->m_numval = 0 - vop1->m_numval;
		break;
	}
	case OP_MINUS:
	{
		//sprintf(str_res,"%f",vop1->m_numval - vop2->m_numval);
		vrst->m_DataType = DT_NUMBER;
		vrst->m_numval = vop1->m_numval - vop2->m_numval;
		break;
	}
	case OP_MULT:
	{
		//sprintf(str_res,"%f",vop1->m_numval * vop2->m_numval);
		vrst->m_DataType = DT_NUMBER;
		vrst->m_numval = vop1->m_numval * vop2->m_numval;
		break;
	}
	case OP_DIVI:
	{
		if (vop2->m_numval == 0)  // devide by 0 error
		{
			//*retcd = ERR_DIVISION_BY_ZERO;
			//sprintf(str_res,"%d", 0);
			//return NULL;
			HDB_RETURN(ERR_DIVISION_BY_ZERO);
			break;
		}
		//sprintf(str_res,"%f", vop1->m_numval / vop2->m_numval);
		vrst->m_DataType = DT_NUMBER;
		vrst->m_numval = vop1->m_numval / vop2->m_numval;
		break;
	}
	case OP_DIV:
	{
		if (vop2->m_numval == 0)  // devide by 0 error
		{
			//*retcd = ERR_DIVISION_BY_ZERO;
			//sprintf(str_res,"%d", 0);
			//return NULL;
			HDB_RETURN(ERR_DIVISION_BY_ZERO);
			break;
		}
		//sprintf(str_res,"%f", floor(vop1->m_numval/vop2->m_numval));
		vrst->m_DataType = DT_NUMBER;
		vrst->m_numval = floor(vop1->m_numval / vop2->m_numval);
		break;
	}
	case OP_MOD:
	{
		if (vop2->m_numval == 0)  // devide by 0 error
		{
			//*retcd = ERR_DIVISION_BY_ZERO;
			//sprintf(str_res,"%d", 0);
			//return NULL;
			HDB_RETURN(ERR_DIVISION_BY_ZERO);
			break;
		}
		//sprintf(str_res,"%f", vop1->m_numval - (vop2->m_numval * floor(vop1->m_numval/vop2->m_numval)));
		vrst->m_DataType = DT_NUMBER;
		vrst->m_numval = vop1->m_numval - (vop2->m_numval * floor(vop1->m_numval / vop2->m_numval));
		break;
	}

	case OP_CAT:
	{
		if (vop1->m_DataType == DT_CHAR && vop2->m_DataType == DT_CHAR) // string
		{
			//strcpy(str_res, vop1->m_strval);
			//strcat(str_res, vop2->m_strval);
			vrst->m_DataType = DT_CHAR;
			vrst->m_strval = (char*)malloc(sizeof(char) * (strlen(vop1->m_strval) + strlen(vop2->m_strval) + 1));
			strcpy(vrst->m_strval, vop1->m_strval);
			strcat(vrst->m_strval, vop2->m_strval);
		}
		else
		{
			//*retcd = ERR_DATATYPEMISMATCH;
			//return NULL;
			HDB_RETURN(ERR_DATATYPEMISMATCH);
		}
		break;
	}
	case OP_EQ:
	{
		//if(op1[0]==39 && op2[0]==39) // string
		if (vop1->m_DataType == DT_CHAR && vop2->m_DataType == DT_CHAR) // string
		{
			cmp = strcmp(vop1->m_strval, vop2->m_strval);
			if (cmp == 0) vrst->m_numval = true; else vrst->m_numval = false;
		}
		//else if(op1[0]!=39 && op2[0]!=39)
		else  if (vop1->m_DataType != DT_CHAR && vop2->m_DataType != DT_CHAR)
		{
			//vop1->m_numval = atol(vop1->m_strval); vop2->m_numval = atol(vop2->m_strval);
			if (vop1->m_numval == vop2->m_numval) vrst->m_numval = true; else vrst->m_numval = false;
			//strcpy(str_res,"true"); else strcpy(str_res,"false");
		}
		else
		{
			//*retcd = ERR_DATATYPEMISMATCH;
			HDB_RETURN(ERR_DATATYPEMISMATCH);
			//return NULL;
		}
		vrst->m_DataType = DT_BOOLEAN;
		break;
	}
	case OP_NE:
	{
		//if(op1[0]==39 && op2[0]==39) // string
		if (vop1->m_DataType == DT_CHAR && vop2->m_DataType == DT_CHAR) // string
		{
			cmp = strcmp(vop1->m_strval, vop2->m_strval);
			if (cmp != 0) vrst->m_numval = true; else vrst->m_numval = false;
		}
		//else  if(op1[0]!=39 && op2[0]!=39)
		else  if (vop1->m_DataType != DT_CHAR && vop2->m_DataType != DT_CHAR)
		{
			//vop1->m_numval = atol(vop1->m_strval); vop2->m_numval = atol(vop2->m_strval);
			if (vop1->m_numval != vop2->m_numval) vrst->m_numval = true; else vrst->m_numval = false;
		}
		else
		{
			HDB_RETURN(ERR_DATATYPEMISMATCH);
		}
		vrst->m_DataType = DT_BOOLEAN;
		break;
	}
	case OP_GT:
	{
		//if(op1[0]==39 && op2[0]==39) // string
		if (vop1->m_DataType == DT_CHAR && vop2->m_DataType == DT_CHAR) // string
		{
			cmp = strcmp(vop1->m_strval, vop2->m_strval);
			if (cmp > 0)  vrst->m_numval = true; else vrst->m_numval = false;
		}
		//else if(op1[0]!=39 && op2[0]!=39)
		else  if (vop1->m_DataType != DT_CHAR && vop2->m_DataType != DT_CHAR)
		{
			//vop1->m_numval = atol(vop1->m_strval); vop2->m_numval = atol(vop2->m_strval);
			if (vop1->m_numval > vop2->m_numval)  vrst->m_numval = true; else vrst->m_numval = false;
		}
		else
		{
			HDB_RETURN(ERR_DATATYPEMISMATCH);
			//return NULL;
		}
		vrst->m_DataType = DT_BOOLEAN;
		break;
	}
	case OP_LT:
	{
		//if(op1[0]==39 && op2[0]==39) // string
		if (vop1->m_DataType == DT_CHAR && vop2->m_DataType == DT_CHAR) // string
		{
			cmp = strcmp(vop1->m_strval, vop2->m_strval);
			if (cmp < 0) vrst->m_numval = true; else vrst->m_numval = false;
		}
		//else  if(op1[0]!=39 && op2[0]!=39)
		else  if (vop1->m_DataType != DT_CHAR && vop2->m_DataType != DT_CHAR)
		{
			//vop1->m_numval = atol(vop1->m_strval); vop2->m_numval = atol(vop2->m_strval);
			if (vop1->m_numval < vop2->m_numval) vrst->m_numval = true; else vrst->m_numval = false;
		}
		else
		{
			HDB_RETURN(ERR_DATATYPEMISMATCH);
			//return NULL;
		}
		vrst->m_DataType = DT_BOOLEAN;
		break;
	}
	case OP_GE:
	{
		//if(op1[0]==39 && op2[0]==39) // string
		if (vop1->m_DataType == DT_CHAR && vop2->m_DataType == DT_CHAR) // string
		{
			cmp = strcmp(vop1->m_strval, vop2->m_strval);
			if (cmp >= 0) vrst->m_numval = true; else vrst->m_numval = false;
		}
		//else  if(op1[0]!=39 && op2[0]!=39)
		else  if (vop1->m_DataType != DT_CHAR && vop2->m_DataType != DT_CHAR)
		{
			//vop1->m_numval = atol(vop1->m_strval); vop2->m_numval = atol(vop2->m_strval);
			if (vop1->m_numval >= vop2->m_numval) vrst->m_numval = true; else vrst->m_numval = false;
		}
		else
		{
			HDB_RETURN(ERR_DATATYPEMISMATCH);
			//return NULL;
		}
		vrst->m_DataType = DT_BOOLEAN;
		break;
	}
	case OP_LE:
	{
		//if(op1[0]==39 && op2[0]==39) // string
		if (vop1->m_DataType == DT_CHAR && vop2->m_DataType == DT_CHAR) // string
		{
			cmp = strcmp(vop1->m_strval, vop2->m_strval);
			if (cmp <= 0) vrst->m_numval = true; else vrst->m_numval = false;
		}
		//else  if(op1[0]!=39 && op2[0]!=39)
		else  if (vop1->m_DataType != DT_CHAR && vop2->m_DataType != DT_CHAR)
		{
			//vop1->m_numval = atol(vop1->m_strval); vop2->m_numval = atol(vop2->m_strval);
			if (vop1->m_numval <= vop2->m_numval) vrst->m_numval = true; else vrst->m_numval = false;
		}
		else
		{
			HDB_RETURN(ERR_DATATYPEMISMATCH);
		}
		vrst->m_DataType = DT_BOOLEAN;
		break;
	}
	case OP_AND:
	{
		//if(EQUAL(vop1->m_strval, "true") && EQUAL(vop2->m_strval, "true"))
		//strcpy(str_res,"true"); else strcpy(str_res,"false");
		if (vop1->m_numval && vop2->m_numval) vrst->m_numval = true; else vrst->m_numval = false;
		vrst->m_DataType = DT_BOOLEAN;
		break;
	}
	case OP_OR:
	{
		//if(EQUAL(vop1->m_strval, "true") || EQUAL(vop2->m_strval, "true"))
		//strcpy(str_res,"true"); else strcpy(str_res,"false");
		if (vop1->m_numval || vop2->m_numval)
			vrst->m_numval = true; else vrst->m_numval = false;
		vrst->m_DataType = DT_BOOLEAN;
		break;
	}
	case OP_NOT:
	{
		//if(EQUAL(vop1->m_strval, "true")) strcpy(str_res,"false"); else strcpy(str_res,"true");
		if (vop1->m_numval)
			vrst->m_numval = false; else vrst->m_numval = true;
		vrst->m_DataType = DT_BOOLEAN;
		break;
	}
	default:HDB_RETURN(ERR_BADOPARATOR);
	}

CLEANUP:
	*pRetCd = retcd;
	return vrst;
}