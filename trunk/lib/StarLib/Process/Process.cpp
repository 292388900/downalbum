#include "stdafx.h"
#include "Process.h"
#include <Shlwapi.h>
#include <Tlhelp32.h>
#include <Psapi.h>

#pragma comment(lib,"Psapi.lib")

/*------------------------------------------------------------------------
[7/24/2009 xiaolin]
说明:	return PID from a process name
		need :
		#include <Shlwapi.h>
		#include <Tlhelp32.h>
------------------------------------------------------------------------*/
ULONG  Star::Process::GetProcessID(char *szProcessName)
{
	DWORD dwProcessId = 0;

	PROCESSENTRY32	ProcessEntry32;
	HANDLE			hSnap;

	ProcessEntry32.dwSize = sizeof (PROCESSENTRY32);

	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hSnap == 0)
		return 0;

	int ret = Process32First(hSnap,&ProcessEntry32);
	while ( ret ){
		if ( StrStrI(ProcessEntry32.szExeFile,szProcessName) ){
			dwProcessId = ProcessEntry32.th32ProcessID;
			break;
		}
		ret = Process32Next(hSnap,&ProcessEntry32);
	}

	CloseHandle(hSnap);

	return dwProcessId;
}

//通过进程ID获取对应文件名
CString Star::Process::GetProcessName(DWORD dwProcessId)
{
	CString			strName;
	PROCESSENTRY32	ProcessEntry32;
	HANDLE			hSnap = INVALID_HANDLE_VALUE;
	int				ret = 0;

	ProcessEntry32.dwSize = sizeof (PROCESSENTRY32);

	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if( hSnap!=INVALID_HANDLE_VALUE ){
		ret = Process32First(hSnap,&ProcessEntry32);
		while ( ret ){
			if ( ProcessEntry32.th32ProcessID==dwProcessId ){
				strName = ProcessEntry32.szExeFile;
				break;
			}
			ret = Process32Next(hSnap,&ProcessEntry32);
		}
		CloseHandle(hSnap);
	}

	return strName;
}

//获取进程对应的文件全路径名
CString Star::Process::GetProcessFileName(DWORD dwProcessId)
{
	TCHAR szFilePath[MAX_PATH] = {0};

	HANDLE hProcess=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,dwProcessId);
	if ( hProcess!=NULL ){
		GetModuleFileNameEx(hProcess,NULL,szFilePath,MAX_PATH);
		CloseHandle(hProcess);
	}

	return szFilePath;
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

BOOL GetModuleName(LPVOID lpImageBase,CString&strModuleName)
{
	BOOL bOK=FALSE;

	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpImageBase;
	if ( pDosHeader->e_magic==IMAGE_DOS_SIGNATURE ){
		PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)( (PBYTE)pDosHeader + pDosHeader->e_lfanew );
		if ( pNtHeader->Signature==IMAGE_NT_SIGNATURE ){
			DWORD dwDataStartRVA=pNtHeader->OptionalHeader.DataDirectory[0].VirtualAddress;
			if ( dwDataStartRVA!=0 ){
				PIMAGE_EXPORT_DIRECTORY pExportDir=(PIMAGE_EXPORT_DIRECTORY)((LPBYTE)lpImageBase+dwDataStartRVA);
				strModuleName = (PCHAR)lpImageBase + pExportDir->Name;
				bOK=TRUE;
			}
		}
	}

	return bOK;
}

//进程要有读内存的权限
BOOL GetModuleName(HANDLE hProcess,LPVOID lpImageBase,DWORD dwImgSize,CString&strModuleName)
{
	BOOL bOK=FALSE;
	const int nPageSize=0x1000;
	DWORD dwReadWrite;
	PBYTE pImageBuff=new BYTE[nPageSize];
	if ( pImageBuff==NULL ){
		return bOK;
	}

	//预读一页
	if ( ::ReadProcessMemory(hProcess,lpImageBase,pImageBuff,nPageSize,&dwReadWrite)==FALSE ){
		delete []pImageBuff;
		return bOK;
	}

	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pImageBuff;
	if ( pDosHeader->e_magic==IMAGE_DOS_SIGNATURE ){
		PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)( (PBYTE)pDosHeader + pDosHeader->e_lfanew );
		if ( pNtHeader->Signature==IMAGE_NT_SIGNATURE ){
			DWORD dwDataStartRVA=pNtHeader->OptionalHeader.DataDirectory[0].VirtualAddress;
			DWORD dwSize=pNtHeader->OptionalHeader.DataDirectory[0].Size;
			if ( dwDataStartRVA!=0 && dwSize!=0 ){
				PIMAGE_EXPORT_DIRECTORY pExportDir=(PIMAGE_EXPORT_DIRECTORY)((LPBYTE)lpImageBase+dwDataStartRVA);
				if ( ::ReadProcessMemory(hProcess,(PCHAR)pExportDir,pImageBuff,sizeof(IMAGE_EXPORT_DIRECTORY),&dwReadWrite) ){
					pExportDir=(PIMAGE_EXPORT_DIRECTORY)pImageBuff;
					dwSize=dwImgSize-pExportDir->Name;
					if ( dwSize>MAX_PATH || (int)dwSize<0 ){
						dwSize=MAX_PATH;
					}
					if ( ::ReadProcessMemory(hProcess,(PCHAR)lpImageBase+pExportDir->Name,pImageBuff,MAX_PATH,&dwReadWrite) ){
						strModuleName = (PCHAR)pImageBuff;
						bOK=TRUE;
					}
				}
			}
		}
	}

	delete []pImageBuff;

	return bOK;
}