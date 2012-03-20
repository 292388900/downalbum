
#pragma once
#include <afx.h>
#include <Tlhelp32.h>

namespace Star
{
	namespace Process
	{
		ULONG GetProcessID(char *szProcessName);

		//通过进程ID获取对应文件名
		CString GetProcessName(DWORD dwProcessId);

		//获取进程对应的文件全路径名
		CString GetProcessFileName(DWORD dwProcessId);

		ULONG GetProcessThreads(ULONG PID,PULONG lpBuffer,ULONG dwBufferSize);
		BOOL WINAPI SuspendProcess(DWORD dwProcessID, BOOL bSuspend);
		HMODULE WINAPI ModuleFromAddress(PVOID pv);
		DWORD WINAPI GetThreadIdFromPID(DWORD dwProcessId);
		BOOL WINAPI GetModuleEntry(DWORD dwProcessID,LPCTSTR lpszModuleName,MODULEENTRY32*pEntry);
		DWORD GetParentProcessID(DWORD dwId);
		DWORD GetParentProcessID2(DWORD dwId);
	}
}