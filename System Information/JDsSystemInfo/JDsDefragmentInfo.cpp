// This is the main DLL file.

#include "stdafx.h"
#include "JDsSystemInfo.h"

#include <strsafe.h>

#define BUFSIZE 4096 
 
HANDLE g_hChildStd_IN_Rd = NULL;
HANDLE g_hChildStd_IN_Wr = NULL;
HANDLE g_hChildStd_OUT_Rd = NULL;
HANDLE g_hChildStd_OUT_Wr = NULL;

HANDLE g_hInputFile = NULL;
 
void CreateChildProcess(void); 
void WriteToPipe(void); 
void ReadFromPipe(void); 
void ErrorExit(PTSTR); 

using namespace JDsSystemInfo;

//
// Test Rollback -04-02-2014
//

void CreateChildProcess();
void ErrorExit(PTSTR lpszFunction); 

void CJDsDefragInfo::GetDefragmentInfo(TCHAR *szBuff)
{
	CreateChildProcess();
}

void CreateChildProcess()
// Create a child process that uses the previously created pipes for STDIN and STDOUT.
{ 
	PROCESS_INFORMATION procHandles;
    STARTUPINFO startWinInfo;
    BOOL    result;

    //TCHAR * cmdname = _T("C:\\Windows\\System32\\cmd.exe");
	TCHAR * cmdname = _T("C:\\Windows\\System32\\defrag.exe");
    TCHAR * cmdargs = _T("/c defrag.exe C: /A");


    DWORD    procFlags;
    DWORD    waitStatus = 0;
    DWORD    procStatus = 0;
    DWORD    winErrCode;
    DWORD    inloop = 1;

    memset(&startWinInfo, 0, sizeof(startWinInfo));
    startWinInfo.cb = sizeof(startWinInfo);
    startWinInfo.lpTitle = _T("Slaved Console");

    procFlags = (CREATE_DEFAULT_ERROR_MODE | CREATE_NEW_CONSOLE | 
        CREATE_NEW_PROCESS_GROUP);

    procHandles.hProcess = INVALID_HANDLE_VALUE;
    procHandles.hThread  = INVALID_HANDLE_VALUE;
    procHandles.dwProcessId = 0;
    procHandles.dwThreadId = 0;

    result = CreateProcess(
        cmdname, cmdargs, NULL, NULL, 0, procFlags, NULL,
        NULL, &startWinInfo, &procHandles);

    if (result == 0)
    {
        winErrCode = GetLastError();
        exit (255);        // error with CreateProcessA()
    }


    waitStatus = WaitForInputIdle(procHandles.hProcess, 6000);
    inloop = GetExitCodeProcess(procHandles.hProcess, &procStatus);

    while (procStatus == STILL_ACTIVE && inloop)
    {
        waitStatus = WaitForSingleObject(procHandles.hProcess, 30000);
        if (waitStatus == WAIT_TIMEOUT)
        {
            inloop = 1;    
        }
        else if (waitStatus == WAIT_OBJECT_0)
        {
            inloop = GetExitCodeProcess(procHandles.hProcess, &procStatus);
        }
        else
        {
            inloop = 0;    // some type of wait abandon or error ...
            exit (254);
        }
    }
    CloseHandle(procHandles.hProcess);
    CloseHandle(procHandles.hThread);

#ifdef AAA
   TCHAR szCmdline[]=TEXT("cmd /c defrag.exe c: /A");
   //TCHAR szCmdline[]=TEXT("cmd.exe");
   PROCESS_INFORMATION piProcInfo; 
   STARTUPINFO siStartInfo;
   BOOL bSuccess = FALSE; 
 
// Set up members of the PROCESS_INFORMATION structure. 
 
   ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );
 
// Set up members of the STARTUPINFO structure. 
// This structure specifies the STDIN and STDOUT handles for redirection.
 
   ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
   siStartInfo.cb = sizeof(STARTUPINFO); 
   //siStartInfo.hStdError = g_hChildStd_OUT_Wr;
   //siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
   //siStartInfo.hStdInput = g_hChildStd_IN_Rd;
   siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
 
// Create the child process. 
    
   bSuccess = CreateProcess(NULL, 
      szCmdline,     // command line 
      NULL,          // process security attributes 
      NULL,          // primary thread security attributes 
      TRUE,          // handles are inherited 
      0,             // creation flags 
      NULL,          // use parent's environment 
      NULL,          // use parent's current directory 
      &siStartInfo,  // STARTUPINFO pointer 
      &piProcInfo);  // receives PROCESS_INFORMATION 
   
   // If an error occurs, exit the application. 
   if ( ! bSuccess ) 
      ErrorExit(TEXT("CreateProcess"));
   else 
   {
      // Close handles to the child process and its primary thread.
      // Some applications might keep these handles to monitor the status
      // of the child process, for example. 

      CloseHandle(piProcInfo.hProcess);
      CloseHandle(piProcInfo.hThread);
   }
#endif
}

void ErrorExit(PTSTR lpszFunction) 

// Format a readable error message, display a message box, 
// and exit from the application.
{ 
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
        (lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)lpszFunction)+40)*sizeof(TCHAR)); 
    StringCchPrintf((LPTSTR)lpDisplayBuf, 
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"), 
        lpszFunction, dw, lpMsgBuf); 
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(1);
}