#include "stdafx.h"
#include "Process.h"
#include <Shlwapi.h>
#include <Tlhelp32.h>

/*------------------------------------------------------------------------
[7/24/2009 xiaolin]
说明:	return PID from a process name
		need :
		#include <Shlwapi.h>
		#include <Tlhelp32.h>
------------------------------------------------------------------------*/
ULONG  Star::Process::GetProcessID(char *szProcessName)
{
	PROCESSENTRY32	ProcessEntry32;
	HANDLE			hSnap;
	int				ret;

	ProcessEntry32.dwSize = sizeof (PROCESSENTRY32);

	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hSnap == 0)
		return 0;

	ret = Process32First(hSnap,&ProcessEntry32);
	while (ret)
	{
		if (StrStrI(ProcessEntry32.szExeFile,szProcessName) )
		{
			return ProcessEntry32.th32ProcessID;
		}
		ret = Process32Next(hSnap,&ProcessEntry32);
	}

	CloseHandle(hSnap);
	return 0;
}

/*------------------------------------------------------------------------
[7/24/2009 xiaolin]
说明: Enum Threads of a process,return size needed
------------------------------------------------------------------------*/
ULONG Star::Process::GetProcessThreads(ULONG PID,PULONG lpBuffer,ULONG dwBufferSize)
{
	THREADENTRY32	ThreadEntry32;
	HANDLE			hSnap;
	int				ret;
	ULONG			count = 0;

	ThreadEntry32.dwSize = sizeof (THREADENTRY32);
	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,PID);
	if(hSnap == 0)
		return 0;

	ret = Thread32First(hSnap,&ThreadEntry32);
	while(ret)
	{
		if (ThreadEntry32.th32OwnerProcessID == PID)
		{
			if (count<dwBufferSize)
				lpBuffer[count] = ThreadEntry32.th32ThreadID;
			count++;
		}    
		ret = Thread32Next(hSnap,&ThreadEntry32);
	}
	CloseHandle(hSnap);
	return count;
}

/*------------------------------------------------------------------------
[7/24/2009 xiaolin]
说明:挂起或唤醒进程的所有线程
------------------------------------------------------------------------*/
BOOL WINAPI Star::Process::SuspendProcess(DWORD dwProcessID, BOOL bSuspend)
{
	// 取得OpenThread函数的地址
	typedef HANDLE (__stdcall *PFNOPENTHREAD)(DWORD, BOOL, DWORD);
	HMODULE hModule = ::GetModuleHandle("kernel32.dll");
	PFNOPENTHREAD OpenThread = (PFNOPENTHREAD)::GetProcAddress(hModule, "OpenThread");
	if(OpenThread == NULL)
		return FALSE;

	// 取得指定进程内的线程列表
	HANDLE hSnap;
	hSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, dwProcessID);
	if(hSnap != INVALID_HANDLE_VALUE)
	{
		// 遍历线程列表
		THREADENTRY32 te = { 0 };
		te.dwSize = sizeof(te);
		BOOL bOK = ::Thread32First(hSnap, &te);
		while(bOK)
		{
			if(te.th32OwnerProcessID == dwProcessID)
			{
				DWORD dwID = te.th32ThreadID;
				// 试图打开这个线程
				HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, dwID);
				if(hThread != NULL)
				{
					// 暂停或者唤醒这个线程
					if(bSuspend)
						::SuspendThread(hThread);
					else
						::ResumeThread(hThread);
					::CloseHandle(hThread);
				}
			}
			bOK = ::Thread32Next(hSnap, &te);
		}
		::CloseHandle(hSnap);
	}

	return TRUE;
}

/*------------------------------------------------------------------------
[7/24/2009 xiaolin]
说明:获取内存地址所在的模块基址
------------------------------------------------------------------------*/
HMODULE WINAPI Star::Process::ModuleFromAddress(PVOID pv) 
{
	MEMORY_BASIC_INFORMATION mbi;
	if(VirtualQuery(pv, &mbi, sizeof(mbi)) != 0)
	{
		return (HMODULE)mbi.AllocationBase;
	}
	else
	{
		return NULL;
	}
}

/*------------------------------------------------------------------------
[2010-1-23 xiaolin]
说明:由进程ID获取相应的主线程ID
------------------------------------------------------------------------*/
DWORD WINAPI Star::Process::GetThreadIdFromPID(DWORD dwProcessId)
{
	HANDLE ThreadHandle;
	THREADENTRY32 ThreadStruct;

	ThreadHandle=CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,dwProcessId);
	ThreadStruct.dwSize=sizeof(ThreadStruct);
	if(Thread32First(ThreadHandle,&ThreadStruct))
	{
		do 
		{
			if (ThreadStruct.th32OwnerProcessID==dwProcessId)
			{
				CloseHandle(ThreadHandle);
				return ThreadStruct.th32ThreadID;
			}
		} while (Thread32Next(ThreadHandle,&ThreadStruct));
	}
	CloseHandle(ThreadHandle);
	return 0;
}

/*------------------------------------------------------------------------
[2010-1-23 xiaolin]
说明:获取进程指定名称的模块信息
------------------------------------------------------------------------*/
BOOL WINAPI Star::Process::GetModuleEntry(DWORD dwProcessID,LPCTSTR lpszModuleName,MODULEENTRY32*pEntry)
{
	BOOL	bFound=FALSE; 
	pEntry->dwSize=sizeof(MODULEENTRY32); 
	HANDLE  hModuleSnap=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,dwProcessID);
	if ( hModuleSnap!=NULL && Module32First(hModuleSnap,pEntry) ){
		do
		{
			if (_stricmp((const char*)pEntry->szModule,lpszModuleName)==0 ){
				bFound=TRUE;
				break;
			}
		}while ( Module32Next(hModuleSnap, pEntry) );
		CloseHandle(hModuleSnap); 
	}
	
	return bFound;	   
}

/*
获取进程的父进程id

// ntdll!NtQueryInformationProcess (NT specific!)
//
// The function copies the process information of the
// specified type into a buffer
//
// NTSYSAPI
// NTSTATUS
// NTAPI
// NtQueryInformationProcess(
//    IN HANDLE ProcessHandle,              // handle to process
//    IN PROCESSINFOCLASS InformationClass, // information type
//    OUT PVOID ProcessInformation,         // pointer to buffer
//    IN ULONG ProcessInformationLength,    // buffer size in bytes
//    OUT PULONG ReturnLength OPTIONAL      // pointer to a 32-bit
//                                          // variable that receives
//                                          // the number of bytes
//                                          // written to the buffer 
// );


*/
DWORD Star::Process::GetParentProcessID(DWORD dwId)
{
	typedef struct
	{
		DWORD ExitStatus;
		DWORD PebBaseAddress;
		DWORD AffinityMask;
		DWORD BasePriority;
		ULONG UniqueProcessId;
		ULONG InheritedFromUniqueProcessId;
	}   PROCESS_BASIC_INFORMATION;

	typedef LONG (WINAPI *PROCNTQSIP)(HANDLE,UINT,PVOID,ULONG,PULONG);
	PROCNTQSIP NtQueryInformationProcess;

	NtQueryInformationProcess = (PROCNTQSIP)GetProcAddress(
		GetModuleHandle("ntdll.dll"),
		"NtQueryInformationProcess"
		);

	if (!NtQueryInformationProcess)
		return 0;

	LONG                      status;
	DWORD                     dwParentPID = (DWORD)-1;
	HANDLE                    hProcess;
	PROCESS_BASIC_INFORMATION pbi;

	// Get process handle
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION,FALSE,dwId);
	if (!hProcess)
		return (DWORD)-1;

	// Retrieve information
	status = NtQueryInformationProcess( hProcess,
		0,
		(PVOID)&pbi,
		sizeof(PROCESS_BASIC_INFORMATION),
		NULL
		);

	// Copy parent Id on success
	if  (!status)
		dwParentPID = pbi.InheritedFromUniqueProcessId;

	CloseHandle (hProcess);

	return dwParentPID;
}

DWORD Star::Process::GetParentProcessID2(DWORD dwId)
{
	DWORD dwppid=0;
	PROCESSENTRY32 pe32={};

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE)
		return dwppid;

	pe32.dwSize = sizeof(PROCESSENTRY32);
	if( Process32First(hSnap, &pe32) ){
		do {
			if( pe32.th32ProcessID == dwId ){
				dwppid = pe32.th32ParentProcessID;
				break;
			}
		} while(Process32Next(hSnap, &pe32));
	}

	CloseHandle(hSnap);
	return dwppid;
}

//通过进程ID获取对应文件名
CString Star::Process::GetProcessNameByPID(DWORD dwId)
{
	CString strName;
	PROCESSENTRY32	ProcessEntry32;
	HANDLE			hSnap;
	int				ret;

	ProcessEntry32.dwSize = sizeof (PROCESSENTRY32);

	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hSnap != 0){
		ret = Process32First(hSnap,&ProcessEntry32);
		while (ret){
			if ( ProcessEntry32.th32ProcessID==dwId  ){
				strName=ProcessEntry32.szExeFile;
				break;
			}
			ret = Process32Next(hSnap,&ProcessEntry32);
		}

		CloseHandle(hSnap);
	}

	return strName;
}