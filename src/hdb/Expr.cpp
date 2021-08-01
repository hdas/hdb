#ifndef __expr_h
#include "expr.h"
#endif

#ifndef __db_h
#include "db.h"
#endif

#ifndef __token_h
#include "token.h"
#endif


/*int icp(int oprtr)
{
	switch(oprtr)
	{
		case OP_CAT      : return 7;
		case OP_UMINUS   : return 7;
		case OP_UPLUS    : return 7;
		case OP_POW      : return 7;

		case OP_MULT     : return 5;
		case OP_DIVI     : return 5;
		case OP_DIV      : return 5;
		case OP_MOD      : return 5;


		case OP_PLUS     : return 4;
		case OP_MINUS    : return 4;

		case OP_NOT      : return 3;

		case OP_EQ       : return 2;
		case OP_GT       : return 2;
		case OP_LT       : return 2;
		case OP_GE       : return 2;
		case OP_LE       : return 2;
		case OP_NE       : return 2;

		case OP_AND      : return 1;
		case OP_OR       : return 1;

		case OP_BRACSTART: return 7;
	}	
	return 10;
}

int isp(int oprtr)
{
	switch(oprtr)
	{
		case OP_CAT      : return 6;
		case OP_UMINUS   : return 6;
		case OP_UPLUS    : return 6;
		case OP_POW      : return 6;

		case OP_MULT     : return 5;
		case OP_DIVI     : return 5;
		case OP_DIV      : return 5;
		case OP_MOD      : return 5;

		case OP_PLUS     : return 4;
		case OP_MINUS    : return 4;

		case OP_NOT      : return 3;

		case OP_EQ       : return 2;
		case OP_GT       : return 2;
		case OP_LT       : return 2;
		case OP_GE       : return 2;
		case OP_LE       : return 2;
		case OP_NE       : return 2;

		case OP_AND      : return 1;
		case OP_OR       : return 1;

		case OP_BRACSTART: return 0;
	}
	return 10;
}*/

/*DBVariable * eval(char * stl[], int st, int lt)
{
	DBVariable * pVar = NULL;
	int res;
	DBStack * mystack=NULL, * pftlist=NULL;
	
	mystack=dCreateStack(&res);
	pftlist=dCreateStack(&res);

	postfix(stl, pftlist, st, lt, NULL, 0);

	// Reverse the stack into mystack
	for(;;)
	{
		pVar = dPopStack(pftlist, &res);
		if(res!=TRUE) break;
		res = dPushStack(mystack, pVar);
		dDestroyVariable(pVar);
		if(res != TRUE) break;
	}
	pVar = eval_pf(mystack,&res);
	dDestroyStack(pftlist);
	dDestroyStack(mystack);
	return pVar;
}*/

/*DBVariable * eval_with_table(DBExpr * the_expr_to_eval, DBTable ** paTable, int nTable, int * pRetcd)
{
	int res=FALSE, retcd=TRUE; //, fld_typ;
	DBVariable * pVar = NULL;
	DBStack * tmplink=NULL;

	
	HDB_ASSERT(the_expr_to_eval != NULL);
	tmplink = the_expr_to_eval->pExprStack;

	// many change to do
	// mainly filling values of variables; this is done by following loop
	
	// Note : the stack is already postfix ed and reversed; so don't alter stack sequence
	for(;;) // the pVar contains only reference; so don't destroy it here; it will used for next record also
	{
		tmplink = tmplink->link; if(tmplink==NULL) break;
		pVar = tmplink->pVar; HDB_ASSERT(pVar != NULL);

		if(pVar->VarClass == VCLS_FIELD) // Just store the value from table
		{
			int typ=DT_UNKNOWN;
			if(pVar->strval!=NULL) free(pVar->strval);
			typ = pVar->DataType; // backup; storing
			pVar->DataType = DT_UNKNOWN;
			pVar->strval = dFetchField(paTable[pVar->ref2], pVar->ref1, &res);
			if(res!=1) HDB_RETURN(res);
			dChangeVariableDatatype(pVar, typ); 
		}
	}

	if(the_expr_to_eval->pExprStack->link->link==NULL) // The expr contains only one value; field
	{
		pVar = dCreateDuplicateVariable(the_expr_to_eval->pExprStack->link->pVar, &res);
	}
	else
	{
		pVar = eval_pf(the_expr_to_eval->pExprStack, &retcd);
	}

CLEANUP:
	*pRetcd = retcd;
	return pVar;
}*/

/*DBVariable * eval_pf(DBStack * pftlist, int * pRetcd)
{
	DBVariable * vtt=NULL, * vop1=NULL, * vop2=NULL, * vrst=NULL;
	int logic_res=true, res=true;
	char bError = false;
	int oprtr;
	DBStack * mystack, * tmplink;

	mystack = dCreateStack(&res);
	
	tmplink = pftlist;

	while(res == true && bError!=true)
	{
		tmplink = tmplink->link;
		if(tmplink==NULL) break;
		vtt = tmplink->pVar; // Just take the reference; because it will not destroy in future; it is used for next record also 
		HDB_ASSERT(vtt != NULL);

		if(vtt->VarClass == VCLS_OPARATOR)
		{
			oprtr = vtt->ref1;
			if( !(oprtr == OP_NOT || oprtr == OP_UPLUS || oprtr == OP_UMINUS) )
			{
				vop2 = dPopStack(mystack, &res);
				if(res!=true) bError = true;
			}
			else vop2 = NULL;

			vop1 = dPopStack(mystack, &res);
			if(res!=1) bError = true;
			if(!bError)
			{
				vrst = PerformOparation(vop1, vop2, oprtr, &logic_res);
				if(logic_res!=true) bError = true;
			}
			if(!bError)
			{
				HDB_ASSERT(vrst!=NULL);
				res = dPushStack(mystack, vrst);
				if(res!=1) bError = true;
				dDestroyVariable(vrst);
			}
			//dDestroyVariable(vtt);
			dDestroyVariable(vop1);
			dDestroyVariable(vop2);
		}
		else dPushStack(mystack, vtt);

		//free(tt);
		//dDestroyVariable(vtt);
		//vtt = dPopStack(pftlist, &res);
	} // of while(res == true && error!=true)
	//free(tt);
	//dDestroyVariable(vtt);

	// ********************** VVI Note : don't destroy vtt here; it is only a reference

	if(!bError)
	{
		vtt = dPopStack(mystack, &res);
		if(res==1)
		{
			*pRetcd = true;
			dDestroyStack(mystack);
			return vtt;
		}
	}
	
	if(logic_res==1) logic_res=ERR_BADEXPR;
	
	*pRetcd = logic_res;
	dDestroyStack(mystack);
	return NULL;
}*/

// postfix function create the post_fix_stack (pftlist) not directly compatible with eval_pf
// it must be reversed before using eval_pf
/*int postfix(char * stl[], DBStack * pftlist, int st, int lt, DBTable ** paTable, int nTable)
{
	DBVariable * cvar=NULL, * tpvar=NULL;
	int oprtr, retcd=false;
	int ti=st;
	int res;
	DBStack * tmpstack = dCreateStack(&res);

	while(ti <= lt)
	{
		cvar = dCreateVariable(&res);
		HDB_ASSERT(stl[ti] != NULL);
		oprtr = dGetOparatorCode(stl[ti]);
		if(!oprtr) // Beacause oprtr is boolean (char); it returns false if bad oparator
		{
			dStoreVariable(cvar, DT_UNKNOWN, stl[ti]);
			cvar->VarClass = isValue(stl[ti]) ? VCLS_VALUE : VCLS_FIELD;
			if(cvar->VarClass==VCLS_VALUE)
			{
				if(stl[ti][0]==39)
					dChangeVariableDatatype(cvar, DT_CHAR);
				else
					dChangeVariableDatatype(cvar, DT_NUMBER);
			}
			else // VCLS_FIELD ; so, compute the field reference
			{
				int ti=0;
				cvar->ref1=-1;
				for(ti=0; ti<nTable; ti++)
				{
					cvar->ref1 = dFieldCode(paTable[ti], cvar->strval);
					//if(cvar->ref1<0) HDB_RETURN(cvar->ref1); // don't do it here
					if(cvar->ref1>=0)
					{
						cvar->DataType = dFieldType(paTable[ti], cvar->ref1);
						cvar->ref2 = ti;
						break;
					}
				}
				if(cvar->ref1<0) HDB_RETURN(ERR_BADFIELD);
			}
		}
		else
		{
			cvar->VarClass = VCLS_OPARATOR;
			cvar->ref1 = oprtr;
			if(ti==st)
			{
				if(EQUAL(stl[ti], "-")) cvar->ref1 = OP_UMINUS;
				if(EQUAL(stl[ti], "+")) cvar->ref1 = OP_UPLUS;
			}
		}

		if(cvar->VarClass != VCLS_OPARATOR) // Current token is a bad oparator i.e. it is a oparand
		{
			dPushStack(pftlist, cvar);
		}
		else if(cvar->ref1 == OP_BRACSTOP) // ")"
		{
			tpvar = dPopStack(tmpstack, &res);
			if(res != true) HDB_RETURN(ERR_BADEXPR);
			while(tpvar->ref1 != OP_BRACSTART)
			{
				dPushStack(pftlist, tpvar);
				dDestroyVariable(tpvar);
				tpvar = dPopStack(tmpstack, &res);
				if(res != true) HDB_RETURN(ERR_BADEXPR);
			} // actually token "(" has come out from the stack adn it is that
		}
		else  // oparator or ')'
		{
			tpvar = dPopStack(tmpstack, &res);
			if(res==true)
			{
				while(isp(tpvar->ref1) >= icp(cvar->ref1))
				{
					dPushStack(pftlist, tpvar);
					dDestroyVariable(tpvar);
					tpvar = dPopStack(tmpstack,&res);
					if(res!=true) break;
				}
				if(res==true) dPushStack(tmpstack, tpvar);// push again tptkn ind free it
			}
			dPushStack(tmpstack, cvar);
		}
		dDestroyVariable(cvar);
		ti++;
	} // of while
	
	// end of expression; empty stack
	for(;;)
	{
		tpvar = dPopStack(tmpstack, &res);
		if(res != true) break;
		dPushStack(pftlist, tpvar);
		dDestroyVariable(tpvar);
	}
	retcd = true;
CLEANUP:
	if(tmpstack!=NULL) dDestroyStack(tmpstack);
	return retcd;
}*/

/*int expr_test(DQuery * qry)
{
	DBVariable * vtt=NULL;
	int res;
	DBStack * mystack=NULL, * pftlist=NULL;

	mystack = dCreateStack(&res);
	pftlist = dCreateStack(&res);

	res = postfix(qry->stl, pftlist, 1, qry->nToken-3, NULL, 0);

	if(res != true)
	{
		printf("Error : %d",res);
	}
	else
	{
		// just reverse pftlist into mystack
		//emptystack(&mystack);
		for(;;)
		{
			vtt = dPopStack(pftlist, &res);
			if(res != true) break;
			res = dPushStack(mystack, vtt);
			dDestroyVariable(vtt);
			if(res != true) break;
		}
		vtt = eval_pf(mystack, &res);
		if(res==true)
		{
			printf("Result : %s\n", vtt->strval);
			//free(tt);
			dDestroyVariable(vtt);
		}
		else return res;
	}
	printf("\n");
	dDestroyStack(mystack);
	dDestroyStack(pftlist);
	return true;
}*/


/*DBVariable * PerformOparation(DBVariable * vop1, DBVariable * vop2, int oprtr, int * pRetcd)
{
	DBVariable * vrst=NULL;
	int cmp, res, retcd=true; // res_datatype=0;
	char str_res[255] = "";
	
	HDB_ASSERT(vop1!=NULL);

	if(vop1->VarClass != VCLS_VALUE && vop1->VarClass != VCLS_FIELD) HDB_RETURN(ERR_BADEXPR);
	// Change to common/generic datatype
	if(vop1->DataType==DT_VARCHAR) vop1->DataType=DT_CHAR;

	// some oparation has only one oparand; so following
	if(oprtr==OP_UMINUS || oprtr==OP_UPLUS || oprtr == OP_NOT)
	{
		if(vop2!=NULL) HDB_RETURN(ERR_BADEXPR);
	}
	else
	{
		HDB_ASSERT(vop2!=NULL);
		if(vop2->VarClass != VCLS_VALUE && vop2->VarClass != VCLS_FIELD) HDB_RETURN(ERR_BADEXPR);
		
		if(vop2->DataType==DT_VARCHAR) vop2->DataType=DT_CHAR;
		if(vop1->DataType != vop2->DataType) HDB_RETURN(ERR_DATATYPEMISMATCH);
	}

	
	
	vrst = dCreateVariable(&res);

	switch(oprtr)
	{
		case OP_PLUS:
			{
				//sprintf(str_res,"%f",vop1->numval + vop2->numval);
				//res_datatype = DT_NUMBER;
				vrst->DataType = DT_NUMBER;
				vrst->numval = vop1->numval + vop2->numval;
				break;
			}
		case OP_UPLUS:
			{
				//sprintf(str_res,"%f",0 + vop1->numval);
				vrst->DataType = DT_NUMBER;
				vrst->numval = vop1->numval;

				break;
			}
		case OP_UMINUS:
			{
				//sprintf(str_res,"%f",0 - vop1->numval);
				vrst->DataType = DT_NUMBER;
				vrst->numval = 0 - vop1->numval;
				break;
			}
		case OP_MINUS:
			{
				//sprintf(str_res,"%f",vop1->numval - vop2->numval);
				vrst->DataType = DT_NUMBER;
				vrst->numval = vop1->numval - vop2->numval;
				break;
			}
		case OP_MULT:
			{
				//sprintf(str_res,"%f",vop1->numval * vop2->numval);
				vrst->DataType = DT_NUMBER;
				vrst->numval = vop1->numval * vop2->numval;
				break;
			}
		case OP_DIVI:
			{
				if(vop2->numval==0)  // devide by 0 error
				{
					//*retcd = ERR_DIVISION_BY_ZERO;
					//sprintf(str_res,"%d", 0);
					//return NULL;
					HDB_RETURN(ERR_DIVISION_BY_ZERO);
					break;
				}
				//sprintf(str_res,"%f", vop1->numval / vop2->numval);
				vrst->DataType = DT_NUMBER;
				vrst->numval = vop1->numval / vop2->numval;
				break;
			}
		case OP_DIV:
			{
				if(vop2->numval==0)  // devide by 0 error
				{
					//*retcd = ERR_DIVISION_BY_ZERO;
					//sprintf(str_res,"%d", 0);
					//return NULL;
					HDB_RETURN(ERR_DIVISION_BY_ZERO);
					break;
				}
				//sprintf(str_res,"%f", floor(vop1->numval/vop2->numval));
				vrst->DataType = DT_NUMBER;
				vrst->numval = floor(vop1->numval/vop2->numval);
				break;
			}
		case OP_MOD:
			{
				if(vop2->numval==0)  // devide by 0 error
				{
					//*retcd = ERR_DIVISION_BY_ZERO;
					//sprintf(str_res,"%d", 0);
					//return NULL;
					HDB_RETURN(ERR_DIVISION_BY_ZERO);
					break;
				}
				//sprintf(str_res,"%f", vop1->numval - (vop2->numval * floor(vop1->numval/vop2->numval)));
				vrst->DataType = DT_NUMBER;
				vrst->numval = vop1->numval - (vop2->numval * floor(vop1->numval/vop2->numval));
				break;
			}

		case OP_CAT:
			{
				if(vop1->DataType==DT_CHAR && vop2->DataType==DT_CHAR) // string
				{
					//strcpy(str_res, vop1->strval);
					//strcat(str_res, vop2->strval);
					vrst->DataType = DT_CHAR;
					vrst->strval = (char*)malloc(sizeof(char) * (strlen(vop1->strval)+strlen(vop2->strval)+1));
					strcpy(vrst->strval, vop1->strval);
					strcat(vrst->strval, vop2->strval);
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
				if(vop1->DataType==DT_CHAR && vop2->DataType==DT_CHAR) // string
				{
					cmp = strcmp(vop1->strval, vop2->strval);
					if(cmp==0) vrst->numval=true; else vrst->numval=false;
				}
				//else if(op1[0]!=39 && op2[0]!=39)
				else  if(vop1->DataType!=DT_CHAR && vop2->DataType!=DT_CHAR)
				{
					//vop1->numval = atol(vop1->strval); vop2->numval = atol(vop2->strval);
					if(vop1->numval==vop2->numval) vrst->numval=true; else vrst->numval=false;
					//strcpy(str_res,"true"); else strcpy(str_res,"false");
				}
				else
				{
					//*retcd = ERR_DATATYPEMISMATCH;
					HDB_RETURN(ERR_DATATYPEMISMATCH);
					//return NULL;
				}
				vrst->DataType = DT_BOOLEAN;
				break;
			}
		case OP_NE:
			{
				//if(op1[0]==39 && op2[0]==39) // string
				if(vop1->DataType==DT_CHAR && vop2->DataType==DT_CHAR) // string
				{
					cmp = strcmp(vop1->strval, vop2->strval);
					if(cmp!=0) vrst->numval=true; else vrst->numval=false;
				}
				//else  if(op1[0]!=39 && op2[0]!=39)
				else  if(vop1->DataType!=DT_CHAR && vop2->DataType!=DT_CHAR)
				{
					//vop1->numval = atol(vop1->strval); vop2->numval = atol(vop2->strval);
					if(vop1->numval!=vop2->numval) vrst->numval=true; else vrst->numval=false;
				}
				else
				{
					HDB_RETURN(ERR_DATATYPEMISMATCH);
				}
				vrst->DataType = DT_BOOLEAN;
				break;
			}
		case OP_GT:
			{
				//if(op1[0]==39 && op2[0]==39) // string
				if(vop1->DataType==DT_CHAR && vop2->DataType==DT_CHAR) // string
				{
					cmp = strcmp(vop1->strval, vop2->strval);
					if(cmp>0)  vrst->numval=true; else vrst->numval=false;
				}
				//else if(op1[0]!=39 && op2[0]!=39)
				else  if(vop1->DataType!=DT_CHAR && vop2->DataType!=DT_CHAR)
				{
					//vop1->numval = atol(vop1->strval); vop2->numval = atol(vop2->strval);
					if(vop1->numval > vop2->numval)  vrst->numval=true; else vrst->numval=false;
				}
				else
				{
					HDB_RETURN(ERR_DATATYPEMISMATCH);
					//return NULL;
				}
				vrst->DataType = DT_BOOLEAN;
				break;
			}
		case OP_LT:
			{
				//if(op1[0]==39 && op2[0]==39) // string
				if(vop1->DataType==DT_CHAR && vop2->DataType==DT_CHAR) // string
				{
					cmp = strcmp(vop1->strval, vop2->strval);
					if(cmp<0) vrst->numval=true; else vrst->numval=false;
				}
				//else  if(op1[0]!=39 && op2[0]!=39)
				else  if(vop1->DataType!=DT_CHAR && vop2->DataType!=DT_CHAR)
				{
					//vop1->numval = atol(vop1->strval); vop2->numval = atol(vop2->strval);
					if(vop1->numval < vop2->numval) vrst->numval=true; else vrst->numval=false;
				}
				else
				{
					HDB_RETURN(ERR_DATATYPEMISMATCH);
					//return NULL;
				}
				vrst->DataType = DT_BOOLEAN;
				break;
			}
		case OP_GE:
			{
				//if(op1[0]==39 && op2[0]==39) // string
				if(vop1->DataType==DT_CHAR && vop2->DataType==DT_CHAR) // string
				{
					cmp = strcmp(vop1->strval, vop2->strval);
					if(cmp>=0) vrst->numval=true; else vrst->numval=false;
				}
				//else  if(op1[0]!=39 && op2[0]!=39)
				else  if(vop1->DataType!=DT_CHAR && vop2->DataType!=DT_CHAR)
				{
					//vop1->numval = atol(vop1->strval); vop2->numval = atol(vop2->strval);
					if(vop1->numval >= vop2->numval) vrst->numval=true; else vrst->numval=false;
				}
				else
				{
					HDB_RETURN(ERR_DATATYPEMISMATCH);
					//return NULL;
				}
				vrst->DataType = DT_BOOLEAN;
				break;
			}
		case OP_LE:
			{
				//if(op1[0]==39 && op2[0]==39) // string
				if(vop1->DataType==DT_CHAR && vop2->DataType==DT_CHAR) // string
				{
					cmp = strcmp(vop1->strval, vop2->strval);
					if(cmp<=0) vrst->numval=true; else vrst->numval=false;
				}
				//else  if(op1[0]!=39 && op2[0]!=39)
				else  if(vop1->DataType!=DT_CHAR && vop2->DataType!=DT_CHAR)
				{
					//vop1->numval = atol(vop1->strval); vop2->numval = atol(vop2->strval);
					if(vop1->numval <= vop2->numval) vrst->numval=true; else vrst->numval=false;
				}
				else
				{
					HDB_RETURN(ERR_DATATYPEMISMATCH);
				}
				vrst->DataType = DT_BOOLEAN;
				break;
			}
		case OP_AND:
			{
				//if(EQUAL(vop1->strval, "true") && EQUAL(vop2->strval, "true"))
					//strcpy(str_res,"true"); else strcpy(str_res,"false");
				if(vop1->numval && vop2->numval) vrst->numval=true; else vrst->numval=false;
				vrst->DataType = DT_BOOLEAN;
				break;
			}
		case OP_OR:
			{
				//if(EQUAL(vop1->strval, "true") || EQUAL(vop2->strval, "true"))
					//strcpy(str_res,"true"); else strcpy(str_res,"false");
				if(vop1->numval || vop2->numval)
				vrst->numval=true; else vrst->numval=false;
				vrst->DataType = DT_BOOLEAN;
				break;
			}
		case OP_NOT:
			{
				//if(EQUAL(vop1->strval, "true")) strcpy(str_res,"false"); else strcpy(str_res,"true");
				if(vop1->numval)
					vrst->numval=false; else vrst->numval=true;
				vrst->DataType = DT_BOOLEAN;
				break;
			}
		default:HDB_RETURN(ERR_BADOPARATOR);
	}

CLEANUP:
	*pRetcd = retcd;
	return vrst;
}*/

/*DBExpr * dCreateExpr(char * stl[], int st, int lt, DBTable ** paTable, int nTable, int * pRetCd)
{
	DBExpr * expr=NULL;
	int retcd=true, res;
	DBStack * rvstk=NULL, * pfstk=NULL;
	DBVariable * pVar=NULL;

	rvstk = dCreateStack(&res);
	pfstk = dCreateStack(&res);

	res = postfix(stl, pfstk, st, lt, paTable, nTable);
	if(res!=1) HDB_RETURN(res);

	// Reverse the stack into rvstk
	for(;;)
	{
		pVar = dPopStack(pfstk, &res);  if(res!=1) if(res==ERR_EOF) break; else HDB_RETURN(res);
		res  = dPushStack(rvstk, pVar); if(res!=1) HDB_RETURN(res);
		res  = dDestroyVariable(pVar);  if(res!=1) HDB_RETURN(res);
	}

	// Now create & filling the Expr
	expr = (DBExpr*)malloc(sizeof(DBExpr));
	expr->pExprStack = rvstk;
	expr->caption = (char*)malloc(strlen(stl[st])+1);
	strcpy(expr->caption, stl[st]);
	
	expr->length = 9;
	if(st==lt)
	{
		if(expr->pExprStack->link->pVar->VarClass==VCLS_FIELD) expr->length = paTable[expr->pExprStack->link->pVar->ref2]->field[expr->pExprStack->link->pVar->ref1].field_length;
		else
		{
			if(expr->pExprStack->link->pVar->VarClass==VCLS_VALUE)
			{
				if(expr->pExprStack->link->pVar->DataType==DT_CHAR) expr->length = strlen(expr->pExprStack->link->pVar->strval);
				else if(expr->pExprStack->link->pVar->DataType==DT_DATETIME) expr->length = 10;
			}
		}
	}

	retcd=true;

CLEANUP:
	dDestroyStack(pfstk);
	*pRetCd = retcd;
	return expr;
}*/

/*int dDestroyExpr(DBExpr * expr)
{
	if(expr==NULL) return true;
	if(expr->caption!=NULL) free(expr->caption);
	if(expr->pExprStack!=NULL) dDestroyStack(expr->pExprStack);
	free(expr);
	return true;
}*/
