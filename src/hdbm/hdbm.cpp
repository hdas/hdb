#include "../hdb/db.h"
#include "hdbm.h"
#include "dbclnt.h"
#include "../hdb/DResult.h"


int main(int argc, char * argv[])
{
	int sd;
	printf("\nWelcome to HDB, a SQL Database Engine Development Project.\n");
	printf("HDB SQL Monitor Release  : %s %s\n", VERSION, VERSION_EXTN);
	printf("Host Platform     : %s\n", HOST_OS);
#ifdef OS_WINDOWS
	printf("Source Timestamp  : %s\n", __TIMESTAMP__);
#endif
	printf("Source Compiled   : %s %s\n\n", __DATE__, __TIME__);

	if (argc < 2)
	{
		printf("Usage : %s <hostname of hdbserver>\n", argv[0]);
		return 3;
	}
	sd = hdb_connect(argv[1]);
	if (sd < 0) return 2;
	sql_prompt(sd);
	return 0;

	printf("Thank you for using HDB.\n");
	return 0;
}


int test()
{
	char str[40];
	printf("Enter a str :");
	scanf("%s", str);
	chop39(str);
	printf("chop39 : %s\n", str);
	return 1;
}

int sql_prompt(int hSock)  /* SQL Command Prompt */
{
	char sql[4000], aline[1024];
	int rr = -1, quit = false;
	int lno, i, sql_end;
	char * tt;
	DResult * result = NULL;

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
				if (EQUAL(tt, ";")) sql_end = true;
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
				//printf("Not implemanted\n");
				rr = ExecuteRunFile(hSock, sql);
				//if(rr==true) printf("Run file success.\n"); else printf("ERROR : %d\n",rr);
				free(tt);
				break;
			}
			free(tt);

			if (sql_end)
			{
				hdb_send_sql(hSock, sql);
				result = hdb_recv_result(hSock);
				result->Output();
				delete result;
				break;
			}
			printf("  %d> ", lno++);
		}
	}
#ifdef OS_WINDOWS
	closesocket(hSock);
#else
	close(hSock);
#endif
	return true;
}



int ExecuteRunFile(int hSock, char * command)
{
	char sql[4000], aline[1024];
	int quit = false;
	int ll, lno, i, sql_end;
	int nRow = 0;
	char * tt;
	char run[30], fname[256];
	DResult * result = NULL;

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
			fgets(aline, 1023, fpin);
			if (feof(fpin)) { quit = true; break; }
			ll = strlen(aline);
			for (i = ll - 1; i >= 0; i--)
			{
				if (aline[i] == 13 || aline[i] == 10) aline[i] = 0;
				else break;
			}

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
				hdb_send_sql(hSock, sql);
				result = hdb_recv_result(hSock);
				result->Output();
				delete result;
				break;
			}
		}
	}
	fclose(fpin);
	return true;
}

