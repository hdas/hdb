#ifndef __sql_h
#include "sql.h"
#endif

#include "DVariable.h"
#include "DResult.h"
#include "DRow.h"
//#include "DSelectQuery.h"
#include "DExpr.h"
#include "DTable.h"
#include "DIndex.h"
#include "DQuery.h"
#include "DSession.h"
#include "DQueryFactory.h"


int SQL()  /* SQL Command Prompt */
{
	char sql[4000], aline[1024];
	int rr = -1, quit = false;
	int lno, i, sql_end;
	char * tt;

	DResult * result = NULL;
	DSession * ssn = NULL;

	ssn = new DSession(DEFAULT_USERNAME);

	while (!quit)
	{
		printf("SQL> ");
		fflush(stdin);
		strcpy(sql, "");
		lno = 2;
		sql_end = false;

		for (;;)
		{
			fgets(aline, 1024, stdin);
			//if(aline[strlen(aline)-1] == '\n') aline[strlen(aline)-1] = 0;
			//if(aline[strlen(aline)-1] == 10) aline[strlen(aline)-1] = 0;
			if (EQUAL(aline, "") || aline[0] == 10 || aline[0] == 13) break;
			strcat(sql, aline);
			for (i = 0;; i++)
			{
				tt = CreateToken(sql, i);
				if (EQUAL(tt, ";")) { sql_end = true; free(tt); break; }
				if (EQUAL(tt, "")) { free(tt); break; }
				free(tt);
			}
			//strcat(sql," ");
			//strcat(sql,aline);

			tt = CreateToken(sql, 0);
			if (strcmp(tt, "QUIT") == 0) { rr = true; quit = true; free(tt); break; }
			if (strcmp(tt, "TOKEN") == 0) { rr = TokenTest(); free(tt); break; }
			if (strcmp(tt, "RUN") == 0)
			{
				rr = RunFile(ssn, sql);
				if (rr == 1) printf("Run file success.\n"); else printf("ERROR : %d\n", rr);
				free(tt);
				break;
			}
			free(tt);

			if (sql_end)
			{
				result = ExecuteSql(ssn, sql);
				// Filling the error message
				if (result->m_errcd < 0)
				{
					char ttsql[1024] = { "Select long_desc from error_code Where Code='" };
					char ecd[16];
					DResult * ttrs = NULL;

					sprintf(ecd, "%d", result->m_errcd);
					strcat(ttsql, ecd);
					strcat(ttsql, "';");

					ttrs = ExecuteSql(ssn, ttsql);
					if (ttrs->m_errcd >= 0)
					{
						if (ttrs->m_nRow >= 1) strcpy(result->m_errmsg, ttrs->m_FirstRow->m_NextRow->m_arValues[0]);
					}
				}
				result->Output();
				delete result;
				break;
			}
			printf("  %d> ", lno++);
		}
	}
	delete ssn;
	return true;
}

int RunFile(DSession * ssn, char * command)
{
	char sql[4000], aline[1024];
	int quit = false;
	int ll, lno, i, sql_end;
	int nRow = 0;
	char * tt;
	char run[30], fname[256];
	DResult * result;

	FILE * fpin;

	sscanf(command, "%s %[^\n]", run, fname);

	fpin = fopen(fname, "rb");
	if (fpin == NULL)
	{
		return ERR_BADFILE;
	}

	while (!quit)
	{
		strcpy(sql, "");
		lno = 2;
		sql_end = false;

		for (;;)
		{
			fgets(aline, 1024, fpin);
			if (feof(fpin)) { quit = true; break; }
			ll = strlen(aline);
			for (i = ll - 1; i >= 0; i--)
			{
				if (aline[i] == 13 || aline[i] == 10) aline[i] = 0;
				else break;
			}
			/*
			if(aline[strlen(aline)-1] == '\n') aline[strlen(aline)-1] = 0;
			if(aline[strlen(aline)-1] == '\n') aline[strlen(aline)-1] = 0;
			*/
			if (strcmp(aline, "") == 0) break;
			for (i = 0;; i++)
			{
				tt = CreateToken(aline, i);
				if (strcmp(tt, ";") == 0) sql_end = true;
				if (strcmp(tt, "") == 0) { free(tt); break; }
				free(tt);
			}
			strcat(sql, " ");
			strcat(sql, aline);

			if (sql_end)
			{
				result = ExecuteSql(ssn, sql);
				if (result->m_errcd < 0)
				{
					char ttsql[1024] = { "Select long_desc from error_code Where Code='" };
					char ecd[16];
					DResult * ttrs = NULL;

					sprintf(ecd, "%d", result->m_errcd);
					strcat(ttsql, ecd);
					strcat(ttsql, "';");

					ttrs = ExecuteSql(ssn, ttsql);
					if (ttrs->m_errcd >= 0)
					{
						if (ttrs->m_nRow >= 1) strcpy(result->m_errmsg, ttrs->m_FirstRow->m_NextRow->m_arValues[0]);
					}
				}
				result->Output();
				delete result;
				break;
			}
		}
	}
	fclose(fpin);
	return true;
}

DResult * ExecuteSql(DSession * ssn, char * sql)
{
	int rr = false, retcd = false;
	DResult * result = NULL;
	DQuery * qry = NULL;

	qry = DQueryFactory::CreateQuery(ssn, sql);

	if (qry == NULL) // if Not any one of upper
	{
		result = new DResult();
		result->m_errcd = ERR_BADSQL;
	}
	else
	{
		if (qry->Parse())
			qry->Execute();

		result = qry->CreateResult();

		delete qry;
	}

	// Filling the error message
	// The code is replaced at different place for Stack Overflow


	return result;
}


