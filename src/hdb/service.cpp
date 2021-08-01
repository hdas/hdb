// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1997  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   service.c
//
//  PURPOSE:  Implements functions required by all services
//            windows.
//
//  FUNCTIONS:
//    main(int argc, char **argv);
//    service_ctrl(DWORD dwCtrlCode);
//    service_main(DWORD dwArgc, LPTSTR *lpszArgv);
//    CmdInstallService();
//    CmdRemoveService();
//    CmdDebugService(int argc, char **argv);
//    ControlHandler ( DWORD dwCtrlType );
//    GetLastErrorText( LPTSTR lpszBuf, DWORD dwSize );
//
//  COMMENTS:
//
//  AUTHOR: Craig Link - Microsoft Developer Support
//


#include "db.h"
#include <direct.h>
#include "service.h"
#include "hdbio.h"
#include "sql.h"

HANDLE  hServerStopEvent = NULL;


// internal variables
SERVICE_STATUS          ssStatus;       // current status of the service
SERVICE_STATUS_HANDLE   sshStatusHandle;
DWORD                   dwErr = 0;
BOOL                    bDebug = FALSE;
TCHAR                   szErr[256];

// internal function prototypes
VOID WINAPI service_ctrl(DWORD dwCtrlCode);
VOID WINAPI service_main(DWORD dwArgc, LPTSTR *lpszArgv);
VOID CmdInstallService();
VOID CmdRemoveService();
VOID CmdDebugService(int argc, char **argv);
BOOL WINAPI ControlHandler(DWORD dwCtrlType);
LPTSTR GetLastErrorText(LPTSTR lpszBuf, DWORD dwSize);

//
//  FUNCTION: main
//
//  PURPOSE: entrypoint for service
//
//  PARAMETERS:
//    argc - number of command line arguments
//    argv - array of command line arguments
//
//  RETURN VALUE:
//    none
//
//  COMMENTS:
//    main() either performs the command line task, or
//    call StartServiceCtrlDispatcher to register the
//    main service thread.  When the this call returns,
//    the service has stopped, so exit.
//
int /*_CRTAPI1*/ main(int argc, char **argv)
{
	SERVICE_TABLE_ENTRY dispatchTable[] =
	{
		{ TEXT(SZSERVICENAME), (LPSERVICE_MAIN_FUNCTION)service_main },
		{ NULL, NULL }
	};

	printf("\nWelcome to HDB, a SQL Database Engine Development Project.\n");
	printf("HDB Release       : %s %s\n", VERSION, VERSION_EXTN);
	printf("Host Platform     : %s\n", HOST_OS);

#ifdef OS_WINDOWS
	printf("Source Timestamp  : %s\n", __TIMESTAMP__);
#endif
	printf("Source Compiled   : %s %s\n\n", __DATE__, __TIME__);

#ifdef OS_WINDOWS
	signal(SIGINT, (void(__cdecl*)(int)) intr_hdlr);
	signal(SIGTERM, (void(__cdecl*)(int)) intr_hdlr);
#else
	signal(SIGHUP, (void (*)()) intr_hdlr);
	signal(SIGQUIT, (void (*)()) intr_hdlr);
	signal(SIGINT, (void (*)()) intr_hdlr);
	signal(SIGTERM, (void (*)()) intr_hdlr);
#endif

	init_global_opt();

	if ((argc > 1) && ((*argv[1] == '-') || (*argv[1] == '/')))
	{
		if (_stricmp("install", argv[1] + 1) == 0)
		{
			CmdInstallService();
		}
		else if (_stricmp("remove", argv[1] + 1) == 0)
		{
			CmdRemoveService();
		}
		else if (_stricmp("debug", argv[1] + 1) == 0)
		{
			bDebug = TRUE;
			CmdDebugService(argc, argv);
		}
		else if (_stricmp("single", argv[1] + 1) == 0)
		{
			SQL();
		}
		else
		{
			goto dispatch;
		}
		printf("Thank you for using HDB.\n");
		exit(0);
	}

	// if it doesn't match any of the above parameters
	// the service control manager may be starting the service
	// so we must call StartServiceCtrlDispatcher
dispatch:
	// this is just to be friendly
	printf("%s -install          to install the service\n", SZAPPNAME);
	printf("%s -remove           to remove the service\n", SZAPPNAME);
	printf("%s -debug <params>   to run as a console app for debugging\n", SZAPPNAME);
	printf("%s -single           to run in single user mode\n", SZAPPNAME);
	printf("\nStartServiceCtrlDispatcher being called.\n");
	printf("This may take several seconds.  Please wait.\n");

	if (!StartServiceCtrlDispatcher(dispatchTable))
		AddToMessageLog(TEXT("StartServiceCtrlDispatcher failed."));
}

void _cdecl intr_hdlr(void)
{
	printf("\n*******Received User Interrupted Signal*******\n");

	printf("Thank you for using HDB.\n");
	exit(0);
}

int init_global_opt()
{
	global_opt.bListen = TRUE;
	global_opt.bLogSql = FALSE;
	sprintf(global_opt.DatabasePath, "%s%s%s", HDB_BASE, DIR_SEP, "data"); // to correct
	global_opt.ListenPort = SERVER_PORT;
	sprintf(global_opt.LogSqlPath, "%s%s%s", HDB_BASE, DIR_SEP, "log");  // to correct
	global_opt.nConnected = 0;
	global_opt.nThread = 0;
	global_opt.bStopRequest = FALSE;

	if (_mkdir(HDB_BASE) == 0) {
		if (_mkdir(global_opt.DatabasePath) == 0) {

		}
	}

	return TRUE;
}
//
//  FUNCTION: service_main
//
//  PURPOSE: To perform actual initialization of the service
//
//  PARAMETERS:
//    dwArgc   - number of command line arguments
//    lpszArgv - array of command line arguments
//
//  RETURN VALUE:
//    none
//
//  COMMENTS:
//    This routine performs the service initialization and then calls
//    the user defined ServiceStart() routine to perform majority
//    of the work.
//
void WINAPI service_main(DWORD dwArgc, LPTSTR *lpszArgv)
{

	// register our service control handler:
	//
	sshStatusHandle = RegisterServiceCtrlHandler(TEXT(SZSERVICENAME), service_ctrl);

	if (!sshStatusHandle)
		goto cleanup;

	// SERVICE_STATUS members that don't change in example
	//
	ssStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	ssStatus.dwServiceSpecificExitCode = 0;


	// report the status to the service control manager.
	//
	if (!ReportStatusToSCMgr(
		SERVICE_START_PENDING, // service state
		NO_ERROR,              // exit code
		3000))                 // wait hint
		goto cleanup;


	ServiceStart(dwArgc, lpszArgv);

cleanup:

	// try to report the stopped status to the service control manager.
	//
	if (sshStatusHandle)
		(VOID)ReportStatusToSCMgr(
		SERVICE_STOPPED,
		dwErr,
		0);

	return;
}



//
//  FUNCTION: service_ctrl
//
//  PURPOSE: This function is called by the SCM whenever
//           ControlService() is called on this service.
//
//  PARAMETERS:
//    dwCtrlCode - type of control requested
//
//  RETURN VALUE:
//    none
//
//  COMMENTS:
//
VOID WINAPI service_ctrl(DWORD dwCtrlCode)
{
	// Handle the requested control code.
	//
	switch (dwCtrlCode)
	{
		// Stop the service.
		//
		// SERVICE_STOP_PENDING should be reported before
		// setting the Stop Event - hServerStopEvent - in
		// ServiceStop().  This avoids a race condition
		// which may result in a 1053 - The Service did not respond...
		// error.
	case SERVICE_CONTROL_STOP:
		ReportStatusToSCMgr(SERVICE_STOP_PENDING, NO_ERROR, 600);
		ServiceStop();
		return;

		// Update the service status.
		//
	case SERVICE_CONTROL_INTERROGATE:
		break;

		// invalid control code
		//
	default:
		break;

	}

	ReportStatusToSCMgr(ssStatus.dwCurrentState, NO_ERROR, 0);
}



//
//  FUNCTION: ReportStatusToSCMgr()
//
//  PURPOSE: Sets the current status of the service and
//           reports it to the Service Control Manager
//
//  PARAMETERS:
//    dwCurrentState - the state of the service
//    dwWin32ExitCode - error code to report
//    dwWaitHint - worst case estimate to next checkpoint
//
//  RETURN VALUE:
//    TRUE  - success
//    FALSE - failure
//
//  COMMENTS:
//
BOOL ReportStatusToSCMgr(DWORD dwCurrentState,
	DWORD dwWin32ExitCode,
	DWORD dwWaitHint)
{
	static DWORD dwCheckPoint = 1;
	BOOL fResult = TRUE;


	if (!bDebug) // when debugging we don't report to the SCM
	{
		if (dwCurrentState == SERVICE_START_PENDING)
			ssStatus.dwControlsAccepted = 0;
		else
			ssStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;

		ssStatus.dwCurrentState = dwCurrentState;
		ssStatus.dwWin32ExitCode = dwWin32ExitCode;
		ssStatus.dwWaitHint = dwWaitHint;

		if ((dwCurrentState == SERVICE_RUNNING) ||
			(dwCurrentState == SERVICE_STOPPED))
			ssStatus.dwCheckPoint = 0;
		else
			ssStatus.dwCheckPoint = dwCheckPoint++;


		// Report the status of the service to the service control manager.
		//
		if (!(fResult = SetServiceStatus(sshStatusHandle, &ssStatus))) {
			AddToMessageLog(TEXT("SetServiceStatus"));
		}
	}
	return fResult;
}



//
//  FUNCTION: AddToMessageLog(LPTSTR lpszMsg)
//
//  PURPOSE: Allows any thread to log an error message
//
//  PARAMETERS:
//    lpszMsg - text for message
//
//  RETURN VALUE:
//    none
//
//  COMMENTS:
//
VOID AddToMessageLog(LPTSTR lpszMsg)
{
	TCHAR   szMsg[256];
	HANDLE  hEventSource;
	LPTSTR  lpszStrings[2];


	if (!bDebug)
	{
		dwErr = GetLastError();

		// Use event logging to log the error.
		//
		hEventSource = RegisterEventSource(NULL, TEXT(SZSERVICENAME));

		_stprintf(szMsg, TEXT("%s error: %d"), TEXT(SZSERVICENAME), dwErr);
		lpszStrings[0] = szMsg;
		lpszStrings[1] = lpszMsg;

		if (hEventSource != NULL) {
			ReportEvent(hEventSource, // handle of event source
				EVENTLOG_ERROR_TYPE,  // event type
				0,                    // event category
				0,                    // event ID
				NULL,                 // current user's SID
				2,                    // strings in lpszStrings
				0,                    // no bytes of raw data
				(const char**)lpszStrings,          // array of error strings
				NULL);                // no raw data

			(VOID)DeregisterEventSource(hEventSource);
		}
	}
}




///////////////////////////////////////////////////////////////////
//
//  The following code handles service installation and removal
//


//
//  FUNCTION: CmdInstallService()
//
//  PURPOSE: Installs the service
//
//  PARAMETERS:
//    none
//
//  RETURN VALUE:
//    none
//
//  COMMENTS:
//
void CmdInstallService()
{
	SC_HANDLE   schService;
	SC_HANDLE   schSCManager;

	TCHAR szPath[512];

	if (GetModuleFileName(NULL, szPath, 512) == 0)
	{
		_tprintf(TEXT("Unable to install %s - %s\n"), TEXT(SZSERVICEDISPLAYNAME), GetLastErrorText(szErr, 256));
		return;
	}

	schSCManager = OpenSCManager(
		NULL,                   // machine (NULL == local)
		NULL,                   // database (NULL == default)
		SC_MANAGER_ALL_ACCESS   // access required
		);
	if (schSCManager)
	{
		schService = CreateService(
			schSCManager,               // SCManager database
			TEXT(SZSERVICENAME),        // name of service
			TEXT(SZSERVICEDISPLAYNAME), // name to display
			SERVICE_ALL_ACCESS,         // desired access
			SERVICE_WIN32_OWN_PROCESS,  // service type
			//SERVICE_DEMAND_START,       // start type
			SERVICE_AUTO_START,       // start type
			SERVICE_ERROR_NORMAL,       // error control type
			szPath,                     // service's binary
			NULL,                       // no load ordering group
			NULL,                       // no tag identifier
			TEXT(SZDEPENDENCIES),       // dependencies
			NULL,                       // LocalSystem account
			NULL);                      // no password

		if (schService)
		{
			_tprintf(TEXT("%s installed.\n"), TEXT(SZSERVICEDISPLAYNAME));
			CloseServiceHandle(schService);
		}
		else
		{
			_tprintf(TEXT("CreateService failed - %s\n"), GetLastErrorText(szErr, 256));
		}

		CloseServiceHandle(schSCManager);
	}
	else
		_tprintf(TEXT("OpenSCManager failed - %s\n"), GetLastErrorText(szErr, 256));
}



//
//  FUNCTION: CmdRemoveService()
//
//  PURPOSE: Stops and removes the service
//
//  PARAMETERS:
//    none
//
//  RETURN VALUE:
//    none
//
//  COMMENTS:
//
void CmdRemoveService()
{
	SC_HANDLE   schService;
	SC_HANDLE   schSCManager;

	schSCManager = OpenSCManager(
		NULL,                   // machine (NULL == local)
		NULL,                   // database (NULL == default)
		SC_MANAGER_ALL_ACCESS   // access required
		);
	if (schSCManager)
	{
		schService = OpenService(schSCManager, TEXT(SZSERVICENAME), SERVICE_ALL_ACCESS);

		if (schService)
		{
			// try to stop the service
			if (ControlService(schService, SERVICE_CONTROL_STOP, &ssStatus))
			{
				_tprintf(TEXT("Stopping %s."), TEXT(SZSERVICEDISPLAYNAME));
				Sleep(1000);

				while (QueryServiceStatus(schService, &ssStatus))
				{
					if (ssStatus.dwCurrentState == SERVICE_STOP_PENDING)
					{
						_tprintf(TEXT("."));
						Sleep(1000);
					}
					else
						break;
				}

				if (ssStatus.dwCurrentState == SERVICE_STOPPED)
					_tprintf(TEXT("\n%s stopped.\n"), TEXT(SZSERVICEDISPLAYNAME));
				else
					_tprintf(TEXT("\n%s failed to stop.\n"), TEXT(SZSERVICEDISPLAYNAME));

			}

			// now remove the service
			if (DeleteService(schService))
				_tprintf(TEXT("%s removed.\n"), TEXT(SZSERVICEDISPLAYNAME));
			else
				_tprintf(TEXT("DeleteService failed - %s\n"), GetLastErrorText(szErr, 256));


			CloseServiceHandle(schService);
		}
		else
			_tprintf(TEXT("OpenService failed - %s\n"), GetLastErrorText(szErr, 256));

		CloseServiceHandle(schSCManager);
	}
	else
		_tprintf(TEXT("OpenSCManager failed - %s\n"), GetLastErrorText(szErr, 256));
}




///////////////////////////////////////////////////////////////////
//
//  The following code is for running the service as a console app
//


//
//  FUNCTION: CmdDebugService(int argc, char ** argv)
//
//  PURPOSE: Runs the service as a console application
//
//  PARAMETERS:
//    argc - number of command line arguments
//    argv - array of command line arguments
//
//  RETURN VALUE:
//    none
//
//  COMMENTS:
//
void CmdDebugService(int argc, char ** argv)
{
	DWORD dwArgc;
	LPTSTR *lpszArgv;

#ifdef UNICODE
	lpszArgv = CommandLineToArgvW(GetCommandLineW(), &(dwArgc) );
#else
	dwArgc = (DWORD)argc;
	lpszArgv = argv;
#endif

	_tprintf(TEXT("Debugging %s.\n"), TEXT(SZSERVICEDISPLAYNAME));

	SetConsoleCtrlHandler(ControlHandler, TRUE);

	ServiceStart(dwArgc, lpszArgv);
}


//
//  FUNCTION: ControlHandler ( DWORD dwCtrlType )
//
//  PURPOSE: Handled console control events
//
//  PARAMETERS:
//    dwCtrlType - type of control event
//
//  RETURN VALUE:
//    True - handled
//    False - unhandled
//
//  COMMENTS:
//
BOOL WINAPI ControlHandler(DWORD dwCtrlType)
{
	switch (dwCtrlType)
	{
	case CTRL_BREAK_EVENT:  // use Ctrl+C or Ctrl+Break to simulate
	case CTRL_C_EVENT:      // SERVICE_CONTROL_STOP in debug mode
		_tprintf(TEXT("Stopping %s.\n"), TEXT(SZSERVICEDISPLAYNAME));
		ServiceStop();
		return TRUE;
		break;

	}
	return FALSE;
}

//
//  FUNCTION: GetLastErrorText
//
//  PURPOSE: copies error message text to string
//
//  PARAMETERS:
//    lpszBuf - destination buffer
//    dwSize - size of buffer
//
//  RETURN VALUE:
//    destination buffer
//
//  COMMENTS:
//
LPTSTR GetLastErrorText(LPTSTR lpszBuf, DWORD dwSize)
{
	DWORD dwRet;
	LPTSTR lpszTemp = NULL;

	dwRet = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ARGUMENT_ARRAY,
		NULL,
		GetLastError(),
		LANG_NEUTRAL,
		(LPTSTR)&lpszTemp,
		0,
		NULL);

	// supplied buffer is not long enough
	if (!dwRet || ((long)dwSize < (long)dwRet + 14))
		lpszBuf[0] = TEXT('\0');
	else
	{
		lpszTemp[lstrlen(lpszTemp) - 2] = TEXT('\0');  //remove cr and newline character
		_stprintf(lpszBuf, TEXT("%s (0x%x)"), lpszTemp, GetLastError());
	}

	if (lpszTemp)
		LocalFree((HLOCAL)lpszTemp);

	return lpszBuf;
}


int ServiceStart(DWORD dwArgc, LPTSTR *lpszArgv)
{
	//    HANDLE                  hPipe = INVALID_HANDLE_VALUE;
	HANDLE                  hEvents[2] = { NULL, NULL };
	//    OVERLAPPED              os;
	//    PSECURITY_DESCRIPTOR    pSD = NULL;
	//    SECURITY_ATTRIBUTES     sa;
	//TCHAR                   szIn[80];
	//TCHAR                   szOut[80];
	//LPTSTR                  lpszPipeName = TEXT("\\\\.\\pipe\\simple");
	//    BOOL                    bRet;
	//    DWORD                   cbRead;
	//    DWORD                   cbWritten;
	DWORD                   dwWait;
	//    UINT                    ndx;

	int sd, err;
	struct sockaddr_in servAddr;
#ifdef OS_WINDOWS
	WORD wVersionRequested;
	WSADATA wsaData;
#else
	int thrd_id;
#endif

	///////////////////////////////////////////////////
	//
	// Service initialization
	//

	// report the status to the service control manager.
	//
	if (!ReportStatusToSCMgr(
		SERVICE_START_PENDING, // service state
		NO_ERROR,              // exit code
		1000))                 // wait hint
		goto cleanup;

	// create the event object. The control handler function signals
	// this event when it receives the "stop" control code.
	//
	hServerStopEvent = CreateEvent(
		NULL,    // no security attributes
		TRUE,    // manual reset event
		FALSE,   // not-signalled
		NULL);   // no name

	if (hServerStopEvent == NULL) goto cleanup;

	hEvents[0] = hServerStopEvent;

	// report the status to the service control manager.
	//
	if (!ReportStatusToSCMgr(
		SERVICE_START_PENDING, // service state
		NO_ERROR,              // exit code
		800))                 // wait hint
		goto cleanup;

	// create the event object object use in overlapped i/o
	//
	hEvents[1] = CreateEvent(
		NULL,    // no security attributes
		TRUE,    // manual reset event
		FALSE,   // not-signalled
		NULL);   // no name

	if (hEvents[1] == NULL) goto cleanup;

	// report the status to the service control manager.
	//
	if (!ReportStatusToSCMgr(
		SERVICE_START_PENDING, // service state
		NO_ERROR,              // exit code
		700))                 // wait hint
		goto cleanup;

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

	/* create socket */
	sd = socket(AF_INET, SOCK_STREAM, 0);
	if (sd < 0)
	{
		perror("Cannot open socket.");
		return -403;
	}

	/* bind server port */
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERVER_PORT);

	if (bind(sd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
	{
		perror("Cannot bind port.");
		return -404;
	}

	// report the status to the service control manager.
	//
	if (!ReportStatusToSCMgr(
		SERVICE_START_PENDING, // service state
		NO_ERROR,              // exit code
		600))                 // wait hint
		goto cleanup;


	if (listen(sd, 5) != 0)
	{
		perror("Can not listen.");
		return -405;
	}


	// report the status to the service control manager.
	//
	if (!ReportStatusToSCMgr(
		SERVICE_RUNNING,       // service state
		NO_ERROR,              // exit code
		200))                    // wait hint
		goto cleanup;

	//
	// End of initialization
	//
	////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////
	//
	// Service is now running, perform work until shutdown
	//

	// Start the server

	while (1)
	{
		// init the overlapped structure
		//
		//memset(&os, 0, sizeof(OVERLAPPED));
		//os.hEvent = hEvents[1];
		ResetEvent(hEvents[1]);

		// wait for a connection...

		if (global_opt.bStopRequest)
		{
			WSACancelBlockingCall();
			break;
		}

		if (global_opt.nThread <= global_opt.nConnected)
		{
#ifdef OS_WINDOWS
			_beginthread(hdb_doio, 0, (void *)(&sd));
#else
			pthread_create((pthread_t*)&thrd_id, NULL, (void*)hdb_doio, (void*)(&sd));
#endif
			global_opt.nThread++;
		}

		//if(GetLastError() == ERROR_IO_PENDING)
		//{
		dwWait = WaitForMultipleObjects(2, hEvents, FALSE, 25);
		if (dwWait == WAIT_TIMEOUT)
		{
		}
		else if (dwWait != WAIT_OBJECT_0 + 1)     // not overlapped i/o event - error occurred,
		{                           // or server stop signaled
			global_opt.bStopRequest = TRUE;
			printf("Request\n");
		}
		//}

		// init the overlapped structure
		//
		//memset( &os, 0, sizeof(OVERLAPPED) );
		//os.hEvent = hEvents[1];
		ResetEvent(hEvents[1]);
	}

	printf("Service Exit\n");

cleanup:

	if (hServerStopEvent) CloseHandle(hServerStopEvent);
	if (hEvents[1]) CloseHandle(hEvents[1]); // overlapped i/o event
	//    if(pSD) free( pSD );
	return TRUE;
}


//
//  FUNCTION: ServiceStop
//
//  PURPOSE: Stops the service
//
//  PARAMETERS:
//    none
//
//  RETURN VALUE:
//    none
//
//  COMMENTS:
//    If a ServiceStop procedure is going to
//    take longer than 3 seconds to execute,
//    it should spawn a thread to execute the
//    stop code, and return.  Otherwise, the
//    ServiceControlManager will believe that
//    the service has stopped responding.
//    
VOID ServiceStop()
{
	if (hServerStopEvent)
		SetEvent(hServerStopEvent);
}

