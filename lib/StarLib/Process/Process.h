
#pragma once
#include <afx.h>
#include <Tlhelp32.h>

namespace Star
{
	namespace Process
	{
		ULONG GetProcessID(char *szProcessName);
		ULONG GetProcessThreads(ULONG PID,PULONG lpBuffer,ULONG dwBufferSize);
		BOOL WINAPI SuspendProcess(DWORD dwProcessID, BOOL bSuspend);
		HMODULE WINAPI ModuleFromAddress(PVOID pv);
		DWORD WINAPI GetThreadIdFromPID(DWORD dwProcessId);
		BOOL WINAPI GetModuleEntry(DWORD dwProcessID,LPCTSTR lpszModuleName,MODULEENTRY32*pEntry);
		DWORD GetParentProcessID(DWORD dwId);
		DWORD GetParentProcessID2(DWORD dwId);
		CString GetProcessNameByPID(DWORD dwId);
	}
}