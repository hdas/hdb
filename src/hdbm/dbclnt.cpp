#ifndef __dbclnt_h
#include "dbclnt.h"
#endif


int hdb_connect(char * hostname)
{
	int sd, rc, err;
	struct sockaddr_in servAddr, localAddr;
#ifdef OS_WINDOWS
	WORD wVersionRequested;
	WSADATA wsaData;
#endif

	struct hostent *h;

#ifdef OS_WINDOWS
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return -401;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		WSACleanup();
		return -402;
	}
#endif

	h = gethostbyname(hostname);
	if (h == NULL)
	{
		printf("Unknown host : '%s'\n", hostname);
		return -410;
	}

	servAddr.sin_family = h->h_addrtype;
	memcpy((char *)&servAddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
	servAddr.sin_port = htons(SERVER_PORT);
	/*

		servAddr.sin_family = AF_INET;
		servAddr.sin_port = htons(SERVER_PORT);
		servAddr.sin_addr.S_un.S_addr = inet_addr(hostname);
		memset(&(servAddr.sin_zero ), 0x00, 8);
		*/

	/* create socket */
	sd = socket(AF_INET, SOCK_STREAM, 0);
	if (sd < 0)
	{
		perror("Cannot open socket ");
		return -411;
	}

	/* bind any port number */
	localAddr.sin_family = AF_INET;
	localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	localAddr.sin_port = htons(0);

	rc = bind(sd, (struct sockaddr *) &localAddr, sizeof(localAddr));
	if (rc < 0)
	{
		printf("Cannot bind port TCP %u\n", SERVER_PORT);
		perror("error ");
		return -412;
	}

	/* connect to server */
	rc = connect(sd, (struct sockaddr *) &servAddr, sizeof(servAddr));
	if (rc < 0)
	{
		printf("Cannot connect hdb server at %s. Server may not running.\n", hostname);
		return  -413;
	}
	printf("\nConnected with HDB Sql Server at %s.\n\n", hostname);
	return sd;
}

int hdb_send_sql(int hSock, char * sql)
{
	char msg[1024] = "";
	char SOH[2] = { 0x01, 0x00 };
	char STX[2] = { 0x02, 0x00 };
	char ETX[2] = { 0x03, 0x00 };
	char EOT[2] = { 0x04, 0x00 };
	char ENQ[2] = { 0x05, 0x00 };
	char EOL[3] = { 0x0D, 0x0A, 0x00 };
	char ver_code[3] = "01", rsp_code[5] = "0001", ssn_code[5] = "0001";

	hdb_sout(hSock, SOH);
	hdb_sout(hSock, ver_code);
	hdb_sout(hSock, rsp_code);
	hdb_sout(hSock, ssn_code);
	hdb_sout(hSock, STX);
	hdb_sout(hSock, sql);
	hdb_sout(hSock, ETX);
	hdb_sout(hSock, EOT);
	return true;
}

DResult * hdb_recv_result(int hSock)
{
#define MAX_RECV 1024
	DResult * result;
	char rcv_buf[MAX_RECV], txt_buf[4096] = "";
	int retcd = false, n;
	int offset = 0, i, req_end = false;
	int bSOH = false, bSTX = false, bETX = false, bEOT = false;
	int iSOH = false, iSTX = false, iETX = false, iEOT = false;
	int ti = 0;
	unsigned int fi = 0, vi = 0;
	int level = 0;
	char * val;


	result = new DResult();

	for (;;)
	{
		n = recv(hSock, rcv_buf, MAX_RECV - 1, 0);
		if (n < 0) { HDB_RETURN(-407); }
		else if (n == 0)
		{
#ifdef OS_WINDOWS
			closesocket(hSock);
#else
			close(hSock);
#endif
			HDB_RETURN(-408);
		}
		rcv_buf[n] = 0x00;
		for (i = 0; i < n; i++)
		{
			if (rcv_buf[i] == 0x01) { bSOH = true; iSOH = i; /*SOH*/ }
			if (rcv_buf[i] == 0x02) { bSTX = true; bETX = false; iSTX = i;/* STX */ }
			if (rcv_buf[i] == 0x03) /* ETX */
			{
				bETX = true;
				bSTX = false;
				iETX = i;
				txt_buf[0] = '>';
				txt_buf[ti] = 0x00;
				ti = 0;
				//printf("%s\n", txt_buf);
				switch (level)
				{
				case 0:{result->m_SqlType = atoi((txt_buf + 1)); level++; break; }
				case 1:{result->m_errcd = atoi((txt_buf + 1)); level++; break; }
				case 2:{strcpy(result->m_errmsg, (txt_buf + 1)); level++; break; }
				case 3:
				{
					result->m_nCol = atoi((txt_buf + 1)); level++;
					result->m_FieldList = (tagDBField*)malloc(result->m_nCol * sizeof(tagDBField));
					break;
				}
				case 4:{result->m_nRow = atoi((txt_buf + 1)); level = 10; break; }

					   // Getting Field
				case 10:
				{
					if (fi == result->m_nCol)
					{
						level = 30; // result->nCol == 0
						break;
					}
					else
					{
						strncpy(result->m_FieldList[fi].m_field_name, (txt_buf + 1), FIELD_NAME_WIDTH - 1);
						level++;
						break;
					}
				}
				case 11:
				{
					result->m_FieldList[fi].m_field_type = atoi((txt_buf + 1));
					level++;
					break;
				}
				case 12:
				{
					result->m_FieldList[fi].m_is_null = atoi((txt_buf + 1));
					level++;
					break;
				}
				/*case 13:
				{
				result->FieldList[fi].is_index = atoi((txt_buf+1));
				level++;
				break;
				}
				case 14:
				{
				result->FieldList[fi].is_unique = atoi((txt_buf+1));
				level++;
				break;
				}*/
				case 13:
				{
					result->m_FieldList[fi].m_field_length = atoi((txt_buf + 1));
					level++;
					break;
				}
				case 14:
				{
					result->m_FieldList[fi].m_field_decimal = atoi((txt_buf + 1));
					level = 10; // Next Field
					fi++;
					if (fi >= result->m_nCol) { level = 20; result->m_nRow = 0; }
					break;
				}

				case 20: // Values
				{
					if (vi >= result->m_nCol) vi = 0;
					if (vi == 0)
						result->AddRow();

					val = (char*)malloc(strlen((txt_buf + 1)) + 1);
					strcpy(val, (txt_buf + 1));
					result->m_cRow->m_arValues[vi++] = val;
					break;
				}
				}
			}
			if (rcv_buf[i] == 0x04) { bEOT = true; iEOT = i;/*<EOT>*/ }

			if (bSTX) txt_buf[ti++] = rcv_buf[i];
		}
		if (bEOT) break;
	}
	retcd = true;

CLEANUP:

	//strcpy(result->retmsg, "Ok");
	//result->retcd=retcd;
	return result;
}

int hdb_sout(int hSock, char * data)
{
	send(hSock, data, strlen(data), 0);
	return true;
}

