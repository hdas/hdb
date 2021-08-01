#ifndef __sql_h
#include "sql.h"
#endif

#ifndef __hdbio_h
#include "hdbio.h"
#endif

#include "DVariable.h"
#include "DQuery.h"

int expr_test(DQuery * qry);
//static void intr_hdlr(void);



/*
int main(int argc,char * argv[])
{
printf("\nWelcome to HDB, a SQL Database Engine Development Project.\n");
printf("HDB Release       : %s %s\n",VERSION, VERSION_EXTN);
printf("Host Platform     : %s\n",HOST_OS);
#ifdef OS_WINDOWS
printf("Source Timestamp  : %s\n",__TIMESTAMP__);
#endif
printf("Source Compiled   : %s %s\n\n",__DATE__, __TIME__);

#ifdef OS_WINDOWS
signal(SIGINT, (void (__cdecl*)(int)) intr_hdlr);
signal(SIGTERM, (void (__cdecl*)(int)) intr_hdlr);
#else
signal(SIGHUP, (void (*)()) intr_hdlr);
signal(SIGQUIT, (void (*)()) intr_hdlr);
signal(SIGINT, (void (*)()) intr_hdlr);
signal(SIGTERM, (void (*)()) intr_hdlr);
#endif

if(argc>1)
{
if(EQUAL(argv[1], "-s"))
{
hdb_server();
}
}
else
{
SQL();
}

printf("Thank you for using HDB.\n");
return 0;
}
*/


int test()
{
	char str[40];
	printf("Enter a str :");
	scanf("%s", str);
	chop39(str);
	printf("chop39 : %s\n", str);
	return 1;
}
/*
int expr_test(DQuery * qry)
{
DVariable * vtt=NULL;
int res;
DStack * mystack=new DStack(), * pftlist=new DStack();

res = pftlist->PopulatePostfix(qry->m_stl, pftlist, 1, qry->m_nToken-3, NULL, 0);

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
vtt = pftlist->Pop();
if(pftlist->m_errcd != SUCCESS) break;
res = mystack->Push(vtt, FALSE);
//dDestroyVariable(vtt);
if(pftlist->m_errcd != SUCCESS) break;
}
vtt = DVariable::eval_pf(mystack, &res);
if(res==true)
{
printf("Result : %s\n", vtt->m_strval);
//free(tt);
delete vtt;
}
else return res;
}
printf("\n");
delete mystack;
delete pftlist;
return true;
}
*/