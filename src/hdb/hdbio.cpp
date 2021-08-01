#ifndef __hdbio_h
#include "hdbio.h"
#endif

#ifndef __sql_h
#include "sql.h"
#endif

//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <netdb.h>


#include "DSession.h"
#include "DRow.h"
#include "DResult.h"


void _cdecl hdb_doio(void * pArgSd)
{
	char rcv_buf[MAX_RCVBUF] = "", req_buf[4096] = "";
	int retcd = true, n;
	int i, cliLen;
	struct sockaddr_in cliAddr;

	unsigned int skt = *((unsigned int*)pArgSd);


#ifndef OS_WINDOWS
	int retval;
#endif
	DSession * ssn = new DSession(DEFAULT_USERNAME);

	printf("Waiting on port TCP %u . . .\n", SERVER_PORT);
	cliLen = sizeof(cliAddr);

	int hSock = accept(skt, (struct sockaddr *)&cliAddr, &cliLen);
	if (hSock < 0)
	{
		perror("Cannot accept connection.");
		return;// -406;
	}

	ssn->SetSockectHandle(hSock);

	printf("Connected with %s :TCP %d.\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));
	global_opt.nConnected++;

	memset(rcv_buf, 0x0, MAX_RCVBUF);

	for (;;)
	{
		n = recv(ssn->GetSockectHandle(), rcv_buf, MAX_RCVBUF - 1, 0);
		if (n < 0)
		{
			printf("\nCannot receive data.\n");
			retcd = -407;
			break;
		}
		else if (n == 0)
		{
			printf("\nConnection closed by client.\n");
#ifdef OS_WINDOWS
			closesocket(ssn->GetSockectHandle());
#else
			close(ssn->hSock);
#endif
			retcd = true;
			break;
		}
		rcv_buf[n] = 0x00;
		if (global_opt.bLogSql) printf("%s", rcv_buf);

		strcat(req_buf, rcv_buf);

		for (i = n - 1; i >= 0; i--)
		{
			if (rcv_buf[i] == 0x04) // <EOT>
			{
				//req_end=true;
				hdb_process_request(ssn, req_buf);
				//printf("\n");
				//fflush(NULL);
				strcpy(req_buf, "");
				break;
			}
		}
	}

	delete ssn;

	//_endthread();
#ifdef OS_WINDOWS
	_endthread();
#else
	pthread_exit((void*)&retval);
#endif
	return;// retcd;
}

int dummy_process_request(DSession * pSsn, char * req_buf)
{
	char buf[64] = { 0x01, 0x30, 0x31, 0x30, 0x30, 0x30, 0x31, 0x30, 0x30, 0x30, 0x31, 0x02,
		0x31, 0x03, 0x02, 0x31, 0x03, 0x02, 0x44, 0x03, 0x02, 0x30, 0x03, 0x02, 0x30, 0x03, 0x0D,
		0x0A, 0x0D, 0x0A, 0x04, 0x00 };
	hdb_sout(pSsn->GetSockectHandle(), buf);
	return true;
}

int hdb_process_request(DSession * pSsn, char * req_buf)
{
	char ver_code[3] = "", req_code[5] = "", ssn_code[5] = "", s_ts[32] = "";
	int retcd = true;
	DResult * result = NULL;
	SYSTEMTIME st;
	FILETIME s_ft, l_ft;
	LARGE_INTEGER s_li, l_li;


	/*time_t st_ltime, nd_ltime;
	time(&st_ltime);*/

	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &s_ft);


	result = ExecuteSql(pSsn, (req_buf + 12));
	// Filling the error_message
	if (result->m_errcd < 0)
	{
		char ttsql[1024] = { "Select long_desc from error_code Where Code='" };
		char ecd[16];
		DResult * ttrs = NULL;

		sprintf(ecd, "%d", result->m_errcd);
		strcat(ttsql, ecd);
		strcat(ttsql, "';");

		ttrs = ExecuteSql(pSsn, ttsql);
		if (ttrs->m_errcd >= 0)
		{
			if (ttrs->m_nRow >= 1) strcpy(result->m_errmsg, ttrs->m_FirstRow->m_NextRow->m_arValues[0]);
		}
	}

	/*time(&nd_ltime);
	sprintf(s_ts, ", %f Sec", difftime(nd_ltime, st_ltime));
	chop_lzero(s_ts);
	strcat(result->retmsg, s_ts);*/
	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &l_ft);
	memcpy(&s_li, &s_ft, sizeof(LARGE_INTEGER));
	memcpy(&l_li, &l_ft, sizeof(LARGE_INTEGER));
	s_li.QuadPart = l_li.QuadPart - s_li.QuadPart;

	memcpy(&s_ft, &s_li, sizeof(FILETIME));
	FileTimeToSystemTime(&s_ft, &st);

	sprintf(s_ts, " @ %d.%d Sec", (st.wHour * 3600) + (st.wMinute * 60) + st.wSecond, st.wMilliseconds);
	strcat(result->m_errmsg, s_ts);


	hdb_output_result(pSsn->GetSockectHandle(), result);
	delete result;

	return retcd;
}

int hdb_output_result(int hSock, DResult * result)
{
	DRow * crow = NULL;
	unsigned int ci;
	//char msg[1024]="";
	char SOH[2] = { 0x01, 0x00 };
	char STX[2] = { 0x02, 0x00 };
	char ETX[2] = { 0x03, 0x00 };
	char EOT[2] = { 0x04, 0x00 };
	char ENQ[2] = { 0x05, 0x00 };
	char EOL[3] = { 0x0D, 0x0A, 0x00 };
	char ver_code[3] = "01", rsp_code[5] = "0001", ssn_code[5] = "0001";
	char buf[2048];



	sprintf(buf, "%s%s%s%s%s%d%s%s%d%s%s%s%s%s%d%s%s%d%s%s", SOH, ver_code,
		rsp_code, ssn_code, STX, result->m_SqlType, ETX, STX, result->m_errcd, ETX,
		STX, result->m_errmsg, ETX, STX, result->m_nCol, ETX, STX, result->m_nRow, ETX, EOL);


	/*
	// Response Header
	hdb_sout(hSock, SOH);
	hdb_sout(hSock, ver_code);
	hdb_sout(hSock, rsp_code);
	hdb_sout(hSock, ssn_code);

	hdb_sout(hSock, STX);
	sprintf(msg, "%d", result->Sql_Type);
	hdb_sout(hSock, msg);
	hdb_sout(hSock, ETX);

	// Error Code & Message
	hdb_sout(hSock, STX);
	sprintf(msg, "%d", result->m_errcd);
	hdb_sout(hSock, msg);
	hdb_sout(hSock, ETX);
	hdb_sout(hSock, STX);
	hdb_sout(hSock, result->retmsg);
	hdb_sout(hSock, ETX);

	// Col & Row
	hdb_sout(hSock, STX);
	sprintf(msg, "%d", result->nCol);
	hdb_sout(hSock, msg);
	hdb_sout(hSock, ETX);
	hdb_sout(hSock, STX);
	sprintf(msg, "%d", result->nRow);
	hdb_sout(hSock, msg);
	hdb_sout(hSock, ETX);
	hdb_sout(hSock, EOL);
	*/

	hdb_sout(hSock, buf);

	// Checking for error
	if (result->m_errcd != true)
	{
		hdb_sout(hSock, EOT);
		return true;
	}

	// displaying field captions
	for (ci = 0; ci < result->m_nCol; ci++)
	{
		strcpy(buf, "");
		sprintf(buf, "%s%s%s%s%d%s%s%d%s%s%d%s%s%d%s%s", STX, result->m_FieldList[ci].m_field_name, ETX, STX, result->m_FieldList[ci].m_field_type, ETX,
			STX, result->m_FieldList[ci].m_is_null, ETX, STX, result->m_FieldList[ci].m_field_length, ETX,
			STX, result->m_FieldList[ci].m_field_decimal, ETX, EOL);

		/*hdb_sout(hSock, STX);
		hdb_sout(hSock, result->FieldList[ci].field_name);
		hdb_sout(hSock, ETX);
		hdb_sout(hSock, STX);
		sprintf(msg, "%d", result->FieldList[ci].field_type);
		hdb_sout(hSock, msg);
		hdb_sout(hSock, ETX);
		hdb_sout(hSock, STX);
		sprintf(msg, "%d", result->FieldList[ci].is_null);
		hdb_sout(hSock, msg);
		hdb_sout(hSock, ETX);
		hdb_sout(hSock, STX);
		sprintf(msg, "%d", result->FieldList[ci].field_length);
		hdb_sout(hSock, msg);
		hdb_sout(hSock, ETX);
		hdb_sout(hSock, STX);
		sprintf(msg, "%d", result->FieldList[ci].field_decimal);
		hdb_sout(hSock, msg);
		hdb_sout(hSock, ETX);
		hdb_sout(hSock, EOL);
		*/
		hdb_sout(hSock, buf);
	}

	hdb_sout(hSock, EOL);

	// displaying result
	crow = result->m_FirstRow->m_NextRow; // beacause first arValue is NULL
	while (crow != NULL)
	{
		for (ci = 0; ci < result->m_nCol; ci++)
		{
			hdb_sout(hSock, STX);
			hdb_sout(hSock, crow->m_arValues[ci]);
			hdb_sout(hSock, ETX);
		}
		hdb_sout(hSock, EOL);
		crow = crow->m_NextRow;
	}
	// End of Transmission
	hdb_sout(hSock, EOT);

	return true;
}

int hdb_sout(int hSock, char * data)
{
	send(hSock, data, strlen(data), 0);
	return true;
}


