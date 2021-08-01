#ifndef __where_h
#include "where.h"
#endif

/*
DBExpr * dCreateWhereClauseExpr(char * stl[], DBTable * tbl, int * result)
{

int wc_st=0, wc_nd=0, tn=1;
int res;
DBExpr * pWc;

for(;;)
{
if(strcmp(stl[tn],"WHERE")==0) { wc_st=tn; break; }
if(strcmp(stl[tn],"")==0) break;
tn++;
}

for(;;)
{
if(strcmp(stl[tn],";")==0) break;
if(strcmp(stl[tn],"GROUP")==0) break;
if(strcmp(stl[tn],"ORDER")==0) break;
if(strcmp(stl[tn],"")==0) break;
wc_nd = tn;
tn++;
}

if(wc_st>0)
{
pWc = (DBExpr*)malloc(sizeof(DBExpr));
pWc->pExprStack = dCreateStack(&res);
postfix(stl, pWc->pExprStack, wc_st+1, wc_nd, NULL, 0);
}
else
{
pWc = NULL;
}
*result = true;
return pWc;
}


int dComputeWhereClauseExpr(DBExpr * pWcExpr, DBTable * tbl, int * retcd) // return boolean
{
int wh_match=false;
int res;
int fieldcode;
DBVariable * vrst=NULL;
int error=false;
if(pWcExpr==NULL)
{
return true;
}
else
{
//DBStack * rev_dup_stack = dCreateStack(&res);
DBStack * cpt_stack = dCreateStack(&res);
DBStack * tmpstack = pWcExpr->pExprStack;
DBVariable * vvar=NULL;
for(;;)
{
tmpstack = tmpstack->link;
if(tmpstack==NULL) break;
vvar = tmpstack->pVar;
if(vvar==NULL) break;
if(vvar->VarClass==VCLS_OPARATOR || vvar->VarClass==VCLS_VALUE)
{
dPushStack(cpt_stack, vvar);
}
else
{
fieldcode = dFieldCode(tbl, vvar->strval);
if(fieldcode >= 0)
{
DBVariable * vtvar=NULL;
char * value;
int fld_typ = dFieldType(tbl, fieldcode);

value = dFetchField(tbl, fieldcode, &res);

vtvar = dCreateVariable(&res);
dStoreVariable(vtvar, fld_typ, value);
dChangeVariableDatatype(vtvar, fld_typ);

dPushStack(cpt_stack, vtvar);
free(value);
}
else
{
*retcd = ERR_BADFIELD;
dDestroyStack(cpt_stack);
return false;
}
}
}

vrst = eval_pf(cpt_stack, &res);
if(res==true)
{
if(vrst->numval) wh_match=true;
else if(!vrst->numval) wh_match=false;
else error = true;
}
else
{
*retcd = res;
error = true;
}

dDestroyStack(cpt_stack);
//dDestroyStack(rev_dup_stack);
dDestroyVariable(vrst);
//free(rst);
}
return wh_match;
}

int dDestroyWhereClauseExpr(DBExpr * pWcExpr)
{
if(pWcExpr!=NULL)
{
dDestroyExpr(pWcExpr);
}
return true;
}

*/

